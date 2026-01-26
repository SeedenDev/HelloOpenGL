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
uniform Material material;

struct Light 
{
    uint type; // 0=global ; 1=directional ; 2=point ; 3=spot
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
uniform Light light;

uniform vec3 u_CameraPos;
uniform float u_Time;

// Just an experiment because I saw this code and it was fun to test
uniform bool u_FogEnabled;
uniform float u_FogMin;
uniform float u_FogMax;

void main()
{
    vec3 baseColor = texture(material.diffuseMap, vertexTex).rgb;
    vec3 specularIntensity = texture(material.specularMap, vertexTex).rgb;
    vec3 emissionColor = texture(material.emissionMap, vertexTex+vec2(0, u_Time)).rgb;
    vec3 emissionMask = step(vec3(1), vec3(1)-specularIntensity);
    emissionColor *= emissionMask;

    vec3 lightFragDir;
    if(light.type!=uint(1)) lightFragDir = normalize(light.position - vertexWorldPos);
    if(light.type==uint(1)) lightFragDir = normalize(light.direction);

    // ambient
    vec3 ambient = baseColor * light.ambient;

    // spotlight first calc
    float theta = dot(lightFragDir, normalize(-light.direction));
    if(light.type==uint(3) && theta <= light.innerCutOff && theta <= light.outerCutOff)
    {
        outColor = vec4(ambient + emissionColor, 1.0);
        return;
    }

    // diffuse
    vec3 normal = normalize(vertexComputedNormal);
    float diffuseStrength = max(dot(normal, lightFragDir), 0.0);
    vec3 diffuse = (baseColor * diffuseStrength) * light.diffuse;

    // specular
    vec3 viewDir = vertexWorldPos - u_CameraPos;
    float dist = length(viewDir);
    viewDir = normalize(viewDir);
    vec3 reflectDir = reflect(lightFragDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (specularIntensity * specularFactor) * light.specular;

    // point light attenuation
    if(light.type==uint(2))
    {
        float lightDist = length(light.position - vertexWorldPos);
        float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist*lightDist));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }
    // spotlight fading
    if(light.type==uint(3))
    {
        float epsilon = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
    }

    vec4 finalColor = vec4(ambient + diffuse + specular + emissionColor, 1.0);

    // Fog test (to move to a special shader when I know how to combine shaders without having to do 2 draw calls)
    if(u_FogEnabled)
    {
        vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
        float focFactor = (u_FogMax - dist) / (u_FogMax - u_FogMin);
        focFactor = clamp(focFactor, 0.0, 1.0);
        finalColor = mix(fogColor, finalColor, focFactor);
    }
    outColor = finalColor;
}