#version 330 core

out vec4 outColor;

in vec4 vertexBasePos;
in vec3 vertexWorldPos;
in vec3 vertexColor;
in vec3 vertexBaseNormal;
in vec3 vertexComputedNormal;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;

uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform int u_SpecularShininess;

uniform bool u_PhongShading;
uniform bool u_WorldSpaceCalc;
uniform mat4 u_Model;
uniform mat4 u_View;

// Just an experiment because I saw this code and it was fun to test
uniform float u_FogMin;
uniform float u_FogMax;

void main()
{
    if(!u_PhongShading)
    {
        outColor = vec4(vertexColor, 1.0);
        return;
    }

    vec3 normal, lightDir, viewDir;

    if(u_WorldSpaceCalc)
    {
        normal = normalize(vertexComputedNormal);
        lightDir = normalize(u_LightPos - vertexWorldPos);
        viewDir = vertexWorldPos - u_CameraPos;
    }
    else
    {
        //  View space calculations (these three could be done in the vertex shader for a performance gain but as it is just a test I don't really care)
        vec3 viewWorldPos = vec3(u_View * u_Model * vertexBasePos);
        vec3 viewComputedNormal = mat3(transpose(inverse(u_View * u_Model))) * vertexBaseNormal;
        vec3 lightPos = vec3(u_View * vec4(u_LightPos, 1.0));

        normal = normalize(viewComputedNormal);
        lightDir = normalize(lightPos - viewWorldPos);
        viewDir = viewWorldPos;
    }
    
    vec3 ambient = u_AmbientStrength * u_LightColor;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * u_LightColor;

    float dist = length(viewDir);
    viewDir = normalize(viewDir);

    vec3 reflectDir = reflect(lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularShininess);
    vec3 specular = u_SpecularStrength * specularFactor * u_LightColor;

    vec3 finalColor = (ambient + diffuse + specular) * vertexColor;

    // Fog test
    vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
    float focFactor = (u_FogMax - dist) / (u_FogMax - u_FogMin);
    focFactor = clamp(focFactor, 0.0, 1.0);

    outColor = mix(fogColor, vec4(finalColor, 1.0), focFactor);
}