#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

out vec4 vertexBasePos;
out vec3 vertexWorldPos;
out vec4 vertexScreenPos;
out vec3 vertexColor;
out vec2 vertexTex;
out vec3 vertexBaseNormal;
out vec3 vertexComputedNormal;

uniform float u_Time;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

// If Phong shading is done there = Gouraud shading
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;

uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform int u_SpecularShininess;

uniform bool u_PhongShading;
uniform float u_FogMin;
uniform float u_FogMax;

void main()
{
    vec4 newPos = u_MVP * pos;
    vec4 worldPos = u_Model * pos;
    vertexComputedNormal = mat3(transpose(inverse(u_Model))) * normal;
    vertexWorldPos = worldPos.xyz;

    if(u_PhongShading)
    {
        vertexBasePos = pos;
        vertexScreenPos = newPos;
        vertexColor = color;
        vertexTex = tex;
        vertexBaseNormal = normal;
    }
    else
    {
        // Gouraud shading
        vec3 ambient = u_AmbientStrength * u_LightColor;

        vertexComputedNormal = normalize(vertexComputedNormal);
        vec3 lightDir = normalize(u_LightPos - vertexWorldPos);
        float diffuseStrength = max(dot(vertexComputedNormal, lightDir), 0.0);
        vec3 diffuse = diffuseStrength * u_LightColor;
        
        vec3 viewDir = vec3(pos) - u_CameraPos;
        float dist = length(viewDir);
        viewDir = normalize(viewDir);

        vec3 reflectDir = reflect(lightDir, vertexComputedNormal);
        float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularShininess);
        vec3 specular = u_SpecularStrength * specularFactor * u_LightColor;

        vec3 finalColor = (ambient + diffuse + specular) * color;

        // Fog test
        vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
        float focFactor = (u_FogMax - dist) / (u_FogMax - u_FogMin);
        focFactor = clamp(focFactor, 0.0, 1.0);

        vertexColor = vec3(mix(fogColor, vec4(finalColor, 1.0), focFactor));
    }
    gl_Position = newPos;
}