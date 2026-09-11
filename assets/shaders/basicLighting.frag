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
    float alpha;
    float reflectivity;
    float ior; // index of refraction
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
    vec4 ambient; // usually low intensity because it's ambient lighting
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
uniform int u_VisualDebugMode;

// Just an experiment because I saw this code and it was fun to test
uniform bool u_FogEnabled;
uniform float u_FogMin;
uniform float u_FogMax;

// Another experiment (from learnopengl.com => depth testing)
float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
float GetDepth()
{
    return LinearizeDepth(gl_FragCoord.z) / far;
}

// Environmental mapping
uniform samplerCube u_Envmap;
vec3 CalcReflectivity()
{
    if(u_Material.reflectivity==0.0) return vec3(0);
    vec3 viewDir = normalize(vertexWorldPos-u_CameraPos);
    vec3 reflectedRay = reflect(viewDir, normalize(vertexComputedNormal));
//doesnt look so much like "how much it reflects" but more "how light/dark is the reflection". Ig PBR will fix this "how much it reflects" and "how clear is the reflection"
    return texture(u_Envmap, reflectedRay).rgb * u_Material.reflectivity;
}
//TODO: add refraction for when the light comes out of the object(which means the normal from which it comes out mhm..), atm single-sided refraction (in)
vec3 CalcRefraction()
{
    if(u_Material.ior==1.0) return vec3(0);
    float srcIOR = 1.0;//air IOR (TODO: from mat to mat refraction, not only air)
    float destIOR = u_Material.ior;
    float ratio = srcIOR / destIOR;
    vec3 viewDir = normalize(vertexWorldPos-u_CameraPos);
    vec3 refractedRay = refract(viewDir, normalize(vertexComputedNormal), ratio);
    return texture(u_Envmap, refractedRay).rgb * 0.8;//eventually, a refraction strength bc pure reflection is too dominant over the ambient color
}

/* Function prototypes */
// Basic lighting
vec4 CalcAmbient(vec3 lightAmbient, vec4 materialAmbient);
vec4 CalcDiffuse(vec3 lightDiffuse, vec4 materialDiffuse, vec3 normal, vec3 lightFragDir);
vec4 CalcSpecular(vec3 lightSpecular, vec4 materialSpecular, vec3 normal, vec3 lightFragDir, vec3 viewDir);
// Light types
vec4 CalcGlobalLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir);
vec4 CalcDirLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir);
vec4 CalcSpotlight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir);
vec4 CalcAttenuationSpotlight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir);

void main()
{
    if(u_VisualDebugMode==1)
    {
        outColor = vec4(vec3(GetDepth()), 1.0);
        return;
    }
    else if(u_VisualDebugMode==2)
    {
        outColor = vec4(normalize(vertexComputedNormal), 1.0);
        return;
    }

    vec4 materialDiffuseColor = u_Material.hasDiffuse ? texture(u_Material.diffuseMap, vertexTex) : vec4(u_Material.diffuseColor, u_Material.alpha);
    if(materialDiffuseColor.a<=0.5) discard; // for sponza scene bruh
    vec4 materialAmbientColor = u_Material.hasDiffuse ? materialDiffuseColor : vec4(u_Material.ambientColor, 1);
    vec4 materialSpecularColor = u_Material.hasSpecular ? texture(u_Material.specularMap, vertexTex) : vec4(u_Material.specularColor, 1);
    materialSpecularColor *= u_Material.specularStrength;
    vec4 materialEmissiveColor = u_Material.hasEmissive ? texture(u_Material.emissiveMap, vertexTex/*+vec2(0, u_Time)*/): vec4(u_Material.emissiveColor, 1);
    //TODO: see if emission should really have a mask depending on the specular in any situation or if it just here (thinking about animated textures, seems a great challenge with emission)
    vec4 emissiveMask = step(vec4(1), vec4(1)-materialSpecularColor);
    materialEmissiveColor *= emissiveMask;
    materialEmissiveColor.a = u_Material.alpha;//fix but maybe temp bc idk if it's right to do it that way

    vec3 normal = normalize(vertexComputedNormal);
    vec3 viewDir = u_CameraPos - vertexWorldPos;
    float viewDist = length(viewDir);
    viewDir = normalize(viewDir);

    vec4 resultColor = vec4(0, 0, 0, u_Material.alpha);

    //TODO: ambient should not be additive. Objects should have an lighting ambient value.
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

    vec4 finalColor = resultColor + materialEmissiveColor;
    finalColor += vec4(CalcReflectivity(), 1);
    finalColor += vec4(CalcRefraction(), 1);

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
vec4 CalcGlobalLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = normalize(light.position.xyz - vertexWorldPos);
    vec4 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec4 diffuse = CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec4 specular = CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}
vec4 CalcDirLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction.xyz);
    vec4 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec4 diffuse = CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightDir);
    vec4 specular = CalcSpecular(light.specular.rgb, materialSpecular, normal, lightDir, viewDir);
    return ambient + diffuse + specular;
}
vec4 CalcPointLight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = light.position.xyz - vertexWorldPos;
    float lightDist = length(lightFragDir);
    lightFragDir = normalize(lightFragDir);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist*lightDist));

    vec4 ambient = attenuation * CalcAmbient(light.ambient.rgb, materialAmbient);
    vec4 diffuse = attenuation * CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec4 specular = attenuation * CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}

/*
uniform mat4 u_View;
uniform mat4 u_Projection;
in vec4 vertexScreenPos;
in vec4 vertexBasePos;
uniform sampler2D u_SpotlightTexture;
*/
vec4 CalcSpotlight(Light light, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = normalize(light.position.xyz - vertexWorldPos);

    float theta = dot(lightFragDir, normalize(-light.direction.xyz));
    // Testing if the spot should lit the fragment
    if(theta <= light.innerCutOff && theta <= light.outerCutOff) return CalcAmbient(light.ambient.rgb, materialAmbient);

    // Then calc the fading
    float epsilon = light.innerCutOff - light.outerCutOff;
    float lightIntensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

    vec4 ambient = CalcAmbient(light.ambient.rgb, materialAmbient);
    vec4 diffuse = lightIntensity * CalcDiffuse(light.diffuse.rgb, materialDiffuse, normal, lightFragDir);
    vec4 specular = lightIntensity * CalcSpecular(light.specular.rgb, materialSpecular, normal, lightFragDir, viewDir);

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

// Basic lighting
vec4 CalcAmbient(vec3 lightAmbient, vec4 materialAmbient)
{
    return vec4(lightAmbient, 1) * materialAmbient;
}

vec4 CalcDiffuse(vec3 lightDiffuse, vec4 materialDiffuse, vec3 normal, vec3 lightFragDir)
{
    float diffuseStrength = max(dot(normal, lightFragDir), 0.0);
    return (materialDiffuse * diffuseStrength) * vec4(lightDiffuse, 1);
}

vec4 CalcSpecular(vec3 lightSpecular, vec4 materialSpecular, vec3 normal, vec3 lightFragDir, vec3 viewDir)
{
    if(u_Material.shininess==0) return vec4(0); // Ignore specular
    vec3 reflectDir = reflect(-lightFragDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    return (materialSpecular * specularFactor) * vec4(lightSpecular, 1);
}