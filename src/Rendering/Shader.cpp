#include "Shader.h"

#include <iostream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include "GlobalUtil.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath)
{
    m_HandlerID = CreateProgram(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader()
{
    glDeleteProgram(m_HandlerID);
}

void Shader::Reload()
{
    const unsigned int rendererID = CreateProgram(m_VertexShaderPath, m_FragmentShaderPath);
    if (rendererID == 0)
    {
        std::cout << "Error: reloading shader program failed!" << std::endl;
        return;
    }
    glDeleteProgram(m_HandlerID);
    m_UniformLocations.clear();
    m_HandlerID = rendererID;
}

void Shader::SetUniform1i(const std::string& name, const int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string& name, const float v0)
{
    glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform2f(const std::string& name, const float v0, const float v1)
{
    glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const std::string& name, const float v0, const float v1, const float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat4)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vec3)
{
    SetUniform3f(name, vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniform1ui(const std::string& name, unsigned int v0)
{
    glUniform1ui(GetUniformLocation(name), v0);
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vec4)
{
    SetUniform4f(name, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::Bind() const
{
    glUseProgram(m_HandlerID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

// private
unsigned int Shader::CreateShader(const GLenum shaderType, const std::string& shaderFilePath)
{
    const std::string fileContent = File::LoadFileContent(shaderFilePath);
    const char* const shaderSrc = fileContent.c_str();
    const unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    /* Error checking */
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = (char*) alloca(length * sizeof(char)); // memory on the stack but freed when leaving the function not the scope /!\ to use with caution
        glGetShaderInfoLog(shader, length, NULL, log);
        std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int Shader::CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    const unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderPath);
    const unsigned int fragShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    /* Error handling */
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* log = (char*) alloca(length * sizeof(char)); // memory on the stack but freed when leaving the function not the scope /!\ to use with caution
        glGetProgramInfoLog(shaderProgram, length, &length, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
        glDeleteProgram(shaderProgram);
        return 0;
    }
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragShader);
    return shaderProgram;
}

//TODO: just cache all the uniform locations (w/name) on shader loading
int Shader::GetUniformLocation(const std::string& name)
{
    if (const auto x = m_UniformLocations.find(name); x != m_UniformLocations.end()) return x->second;
    
    int location = glGetUniformLocation(m_HandlerID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform " << name << " not found!" << std::endl;

    m_UniformLocations.emplace(name, location); // [name] = location;
    return location;
}