#version 330 core

out vec4 outColor;

in vec3 vertexWorldPos;
in vec3 vertexComputedNormal;

struct Material 
{
    vec3 ambient; // color under ambient lightning => surface color
    vec3 diffuse; // color under diffuse lightning => surface color too
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

// Just an experiment because I saw this code and it was fun to test
uniform float u_FogMin;
uniform float u_FogMax;

void main()
{
    vec3 normal = normalize(vertexComputedNormal);
    vec3 lightDir = normalize(light.position - vertexWorldPos);
    vec3 viewDir = vertexWorldPos - u_CameraPos;
    
    vec3 ambient = material.ambient * light.ambient;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = (material.diffuse * diffuseStrength) * light.diffuse;

    float dist = length(viewDir);
    viewDir = normalize(viewDir);

    vec3 reflectDir = reflect(lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * specularFactor) * light.specular;

    vec3 finalColor = ambient + diffuse + specular;

    // Fog test (to move to a special shader when I know how to combine shaders without having to do 2 draw calls)
    vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
    float focFactor = (u_FogMax - dist) / (u_FogMax - u_FogMin);
    focFactor = clamp(focFactor, 0.0, 1.0);

    outColor = mix(fogColor, vec4(finalColor, 1.0), focFactor);
}