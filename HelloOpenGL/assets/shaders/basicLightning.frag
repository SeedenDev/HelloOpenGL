#version 330 core

out vec4 outColor;

in vec2 vertexTex;
in vec3 vertexWorldPos;
in vec3 vertexComputedNormal;

struct Material 
{
    sampler2D diffuseMap; // stores both ambient&diffuse color as the object texture
    sampler2D specularMap; // used for specular sampling
    sampler2D emissionMap; // emission = glowing even if not lit
    vec3 specular; // color of specular highlight or reflect a surface-specific color
    float shininess; // scattering
};
uniform Material u_Material;

struct Light 
{
    vec3 position; // with global/point/spot
    vec3 direction; // with directional/spot
    vec3 ambient; // usually low intensity because it's ambient lightning
    vec3 diffuse; // the exact color of the light
    vec3 specular; // shining intensity
    // point light attenuation parameters
    float constant;
    float linear;
    float quadratic;
    // spotlight radius
    float innerCutOff;
    float outerCutOff;
};
//TODO: not fixed values
uniform int u_GlobalLightCount;
uniform Light u_GlobalLights[10];
uniform int u_DirectionalLightCount;
uniform Light u_DirectionalLights[10];
uniform int u_PointLightCount;
uniform Light u_PointLights[10];
uniform int u_SpotlightCount;
uniform Light u_Spotlights[10];

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

#define LIGHT(type) for(int i = 0; i < u_##type##LightCount; i++) finalColor += Calc##type##Light(u_GlobalLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);

void main()
{
    vec3 materialBaseColor = texture(u_Material.diffuseMap, vertexTex).rgb;
    vec3 materialSpecularIntensity = texture(u_Material.specularMap, vertexTex).rgb;
    //TODO: see if emission should really have a mask depending on the specular in any situation or if it just here (thinking about animated textures, seems a great challenge with emission)
    vec3 materialEmissionColor = texture(u_Material.emissionMap, vertexTex+vec2(0, u_Time)).rgb;
    vec3 emissionMask = step(vec3(1), vec3(1)-materialSpecularIntensity);
    materialEmissionColor *= emissionMask;

    vec3 normal = normalize(vertexComputedNormal);
    vec3 viewDir = vertexWorldPos - u_CameraPos;
    float viewDist = length(viewDir);
    viewDir = normalize(viewDir);

    vec3 resultColor = vec3(0);

    for(int i = 0; i < u_GlobalLightCount; i++) resultColor += CalcGlobalLight(u_GlobalLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_DirectionalLightCount; i++) resultColor += CalcDirLight(u_DirectionalLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_PointLightCount; i++) resultColor += CalcPointLight(u_PointLights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);
    for(int i = 0; i < u_SpotlightCount; i++) resultColor += CalcSpotlight(u_Spotlights[i], materialBaseColor, materialBaseColor, materialSpecularIntensity, normal, viewDir);

    vec4 finalColor = vec4(resultColor + materialEmissionColor, 1.0);

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
    vec3 lightFragDir = normalize(light.position - vertexWorldPos);
    vec3 ambient = CalcAmbient(light.ambient, materialAmbient);
    vec3 diffuse = CalcDiffuse(light.diffuse, materialDiffuse, normal, lightFragDir);
    vec3 specular = CalcSpecular(light.specular, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}
vec3 CalcDirLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    vec3 ambient = CalcAmbient(light.ambient, materialAmbient);
    vec3 diffuse = CalcDiffuse(light.diffuse, materialDiffuse, normal, lightDir);
    vec3 specular = CalcSpecular(light.specular, materialSpecular, normal, lightDir, viewDir);
    return ambient + diffuse + specular;
}
vec3 CalcPointLight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = light.position - vertexWorldPos;
    float lightDist = length(lightFragDir);
    lightFragDir = normalize(lightFragDir);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist*lightDist));

    vec3 ambient = attenuation * CalcAmbient(light.ambient, materialAmbient);
    vec3 diffuse = attenuation * CalcDiffuse(light.diffuse, materialDiffuse, normal, lightFragDir);
    vec3 specular = attenuation * CalcSpecular(light.specular, materialSpecular, normal, lightFragDir, viewDir);
    return ambient + diffuse + specular;
}
vec3 CalcSpotlight(Light light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, vec3 normal, vec3 viewDir)
{
    vec3 lightFragDir = normalize(light.position - vertexWorldPos);

    float theta = dot(lightFragDir, normalize(-light.direction));
    // Testing if the spot should lit the fragment
    if(theta <= light.innerCutOff && theta <= light.outerCutOff) return CalcAmbient(light.ambient, materialAmbient);

    // Then calc the fading
    float epsilon = light.innerCutOff - light.outerCutOff;
    float lightIntensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

    vec3 ambient = CalcAmbient(light.ambient, materialAmbient);
    vec3 diffuse = lightIntensity * CalcDiffuse(light.diffuse, materialDiffuse, normal, lightFragDir);
    vec3 specular = lightIntensity * CalcSpecular(light.specular, materialSpecular, normal, lightFragDir, viewDir);

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