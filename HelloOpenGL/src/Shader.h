#pragma once

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader
{
private:
    std::string m_VertexShaderPath, m_FragmentShaderPath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocations;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

    void Reload();

    void SetUniform1i(const std::string& name, int v0);
    void SetUniform1f(const std::string& name, float v0);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& mat4);
    void SetUniformVec3f(const std::string& name, const glm::vec3& vec3);

	void Bind() const;
	void Unbind() const;

private:
    std::string ParseShaderFile(const std::string& filepath);
    unsigned int CreateShader(const GLenum shaderType, const std::string& filepath);
    unsigned int CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    int GetUniformLocation(const std::string& name);
};