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
    vec3 position;
    vec3 ambient; // usually low intensity because it's ambient lightning
    vec3 diffuse; // the exact color of the light
    vec3 specular; // shining intensity
};
uniform Light light;

uniform vec3 u_CameraPos;
uniform float u_Time;

// Just an experiment because I saw this code and it was fun to test
uniform float u_FogMin;
uniform float u_FogMax;

void main()
{
    vec3 baseColor = texture(material.diffuseMap, vertexTex).rgb;
    vec3 specularIntensity = texture(material.specularMap, vertexTex).rgb;
    vec3 emissionColor = texture(material.emissionMap, vertexTex+vec2(0, u_Time)).rgb;
    vec3 emissionMask = step(vec3(1), vec3(1)-specularIntensity);
    emissionColor *= emissionMask;

    // ambient
    vec3 ambient = baseColor * light.ambient;

    // diffuse
    vec3 normal = normalize(vertexComputedNormal);
    vec3 lightDir = normalize(light.position - vertexWorldPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = (baseColor * diffuseStrength) * light.diffuse;

    // specular
    vec3 viewDir = vertexWorldPos - u_CameraPos;
    float dist = length(viewDir);
    viewDir = normalize(viewDir);
    vec3 reflectDir = reflect(lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (specularIntensity * specularFactor) * light.specular;

    vec3 finalColor = ambient + diffuse + specular + emissionColor;

    // Fog test (to move to a special shader when I know how to combine shaders without having to do 2 draw calls)
    vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
    float focFactor = (u_FogMax - dist) / (u_FogMax - u_FogMin);
    focFactor = clamp(focFactor, 0.0, 1.0);

    outColor = mix(fogColor, vec4(finalColor, 1.0), focFactor);
}