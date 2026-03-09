#version 430 core

out vec4 outColor;

in vec2 vertexTex;
in vec3 vertexWorldPos;
in vec3 vertexComputedNormal;

struct Material 
{
    bool hasDiffuse;
    bool hasSpecular;
    bool hasEmissive;
    // Material properties
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 emissiveColor;
    float shininess; // scattering
    float specularStrength; // scales the specularColor
    // Eventual textures
    sampler2D diffuseMap; // stores both ambient&diffuse color as the object texture
    sampler2D specularMap; // used for specular sampling
    sampler2D emissiveMap; // emissive = glowing even if not lit
};
uniform Material u_Material;

struct Light 
{
    vec4 position; // with global/point/spot
    vec4 direction; // with directional/spot
    vec4 ambient; // usually low intensity because it's ambient lightning
    vec4 diffuse; // the exact color of the light
    vec4 specular; // shining intensity
    // point light attenuation parameters
    float constant;
    float linear;
    float quadratic;
    // spotlight radius
    float innerCutOff;
    float outerCutOff;
    float type; // 0=global ; 1=dir ; 2=point ; 3=spot ; 4=flash
    float a; //TODO: remove this bruh.. but it's for the data layout alignment.... have to find a better fix
    float b;
};

uniform int u_LightCount;
layout(std430) buffer u_LightsBuffer
{
    Light u_Lights[];
};
/*
uniform int u_GlobalLightCount;
uniform Light u_GlobalLights[10];
uniform int u_DirectionalLightCount;
uniform Light u_DirectionalLights[10];
uniform int u_PointLightCount;
uniform Light u_PointLights[10];
uniform int u_SpotlightCount;
uniform Light u_Spotlights[10];*/

uniform vec3 u_CameraPos;
uniform float u_Time;

// Just an experiment because I saw this code and it was fun to test
uniform bool u_FogEnabled;
uniform float u_FogMin;
uniform float u_FogMax;

/* Function prototypes */
// Basic lightning
vec3 CalcAmbient(vec3 lightAmbient, vec3 materialAmbient);
vec3 CalcDiffuse(vec3 lightDiffuse, vec3 materialDiffuse, vec3 normal, vec3 lightFragDir);
vec3 CalcSpecular(vec3 lightSpecular, vec3 materialSpecular, vec3 normal, vec3 lightFragDir, vec3 viewDir);
// Light types
vec3 CalcGlobalLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir);
vec3 CalcDirLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir);
vec3 CalcSpotlight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir);
vec3 CalcAttenuationSpotlight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir);

void main()
{
    //TODO: use alpha for light calculation???
    vec4 materialDiffuseColorAlpha = u_Material.hasDiffuse ? texture(u_Material.diffuseMap, vertexTex).rgba : vec4(u_Material.diffuseColor, 1);
    if(materialDiffuseColorAlpha.a<0.5) discard; // for sponza scene bruh
    vec3 materialDiffuseColor = materialDiffuseColorAlpha.rgb;
    vec3 materialAmbientColor = u_Material.hasDiffuse ? materialDiffuseColor : u_Material.ambientColor;
    vec3 materialSpecularColor = u_Material.hasSpecular ? texture(u_Material.specularMap, vertexTex).rgb : u_Material.specularColor; 
    materialSpecularColor *= u_Material.specularStrength;
    vec3 materialEmissiveColor = u_Material.hasEmissive ? texture(u_Material.emissiveMap, vertexTex/*+vec2(0, u_Time)*/).rgb : u_Material.emissiveColor;
    //TODO: see if emission should really have a mask depending on the specular in any situation or if it just here (thinking about animated textures, seems a great challenge with emission)
    vec3 emissiveMask = step(vec3(1), vec3(1)-materialSpecularColor);
    materialEmissiveColor *= emissiveMask;

    vec3 normal = normalize(vertexComputedNormal);
    vec3 viewDir = vertexWorldPos - u_CameraPos;
    float viewDist = length(viewDir);
    viewDir = normalize(viewDir);

    vec3 resultColor = vec3(0);

    for(int i = 0; i < u_LightCount; i++)
    {
        Light light = u_Lights[i];
        if(light.type==0) resultColor += CalcGlobalLight(light, materialAmbientColor, materialDiffuseColor, materialSpecularColor, normal, viewDir);
        else if(light.type==1) resultColor += CalcDirLight(light, materialAmbientColor, materialDiffuseColor, materialSpecularColor, normal, viewDir);
        else if(light.type==2) resultColor += CalcPointLight(light, materialAmbientColor, materialDiffuseColor, materialSpecularColor, normal, viewDir);
        else if(light.type==3 || light.type==4) resultColor += CalcSpotlight(light, materialAmbientColor, materialDiffuseColor, materialSpecularColor, normal, viewDir);
    }
    /*
    for(int i = 0; i < u_GlobalLightCount; i++) resultColor += CalcGlobalLight(u_GlobalLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_DirectionalLightCount; i++) resultColor += CalcDirLight(u_DirectionalLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_PointLightCount; i++) resultColor += CalcPointLight(u_PointLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_SpotlightCount; i++) resultColor += CalcSpotlight(u_Spotlights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    */

    vec4 finalColor = vec4(resultColor + materialEmissiveColor, 1.0);

    // Fog test (to move to a special shader when I know how to combine shaders without having to do 2 draw calls)
    if(u_FogEnabled)
    {
        vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
        float focFactor = (u_FogMax - viewDist) / (u_FogMax - u_FogMin);
        focFactor = clamp(focFactor, 0.0, 1.0);
        finalColor = mix(fogColor, finalColor, focFactor);
    }
    outColor = finalColor;
}

// Light types
vec3 CalcGlobalLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = normalize(light.position.xyz - vertexWorldPos);
    vec3 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec3 diffuse = CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec3 specular = CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}
vec3 CalcDirLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction.xyz);
    vec3 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec3 diffuse = CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightDir);
    vec3 specular = CalcSpecular(light.specular.rgb, materialSpecular, normal, lightDir, viewDir);
    return ambient + diffuse + specular;
}
vec3 CalcPointLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = light.position.xyz - vertexWorldPos;
    float lightDist = length(lightFragDir);
    lightFragDir = normalize(lightFragDir);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist*lightDist));

    vec3 ambient = attenuation * CalcAmbient(light.ambient.rgb, materialAmbient);
    vec3 diffuse = attenuation * CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec3 specular = attenuation * CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}

/*
uniform mat4 u_View;
uniform mat4 u_Projection;
in vec4 vertexScreenPos;
in vec4 vertexBasePos;
uniform sampler2D u_SpotlightTexture;
*/
vec3 CalcSpotlight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = normalize(light.position.xyz - vertexWorldPos);

    float theta = dot(lightFragDir, normalize(-light.direction.xyz));
    // Testing if the spot should lit the fragment
    if(theta <= light.innerCutOff && theta <= light.outerCutOff) return CalcAmbient(light.ambient.rgb, materialAmbient);

    // Then calc the fading
    float epsilon = light.innerCutOff - light.outerCutOff;
    float lightIntensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

    vec3 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec3 diffuse = lightIntensity * CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec3 specular = lightIntensity * CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);

    // Texture projection (for future impl: https://en.wikibooks.org/wiki/GLSL_Programming/Unity/Cookies)
    /*vec4 textureProjection = (u_Projection * u_View * light.position) * vec4(vertexWorldPos, 1.0);
    textureProjection /= textureProjection.w; // rasterize the projection space
    textureProjection = textureProjection * 0.5 + 0.5; // clip it [0;1] range
    vec2 textureCoords = textureProjection.xy;//clamp(textureProjection.xy, vec2(0), vec2(1));

    bool inFrustum = all(greaterThanEqual(textureCoords, vec2(0.0))) && all(lessThanEqual(textureCoords, vec2(1.0)));

    //vec2 fragCoord = (gl_FragCoord.xy / vec2(1920, 1017));

    vec3 spotTexture = lightIntensity * vec3(texture(u_SpotlightTexture, textureCoords));*/

    return ambient + diffuse + specular;
}

// Basic lightning
vec3 CalcAmbient(vec3 lightAmbient, vec3 materialAmbient)
{
    return lightAmbient * materialAmbient;
}

vec3 CalcDiffuse(vec3 lightDiffuse, vec3 materialDiffuse, vec3 normal, vec3 lightFragDir)
{
    float diffuseStrength = max(dot(normal, lightFragDir), 0.0);
    return (materialDiffuse * diffuseStrength) * lightDiffuse;
}

vec3 CalcSpecular(vec3 lightSpecular, vec3 materialSpecular, vec3 normal, vec3 lightFragDir, vec3 viewDir)
{
    vec3 reflectDir = reflect(lightFragDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    return (materialSpecular * specularFactor) * lightSpecular;
}