#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath)
{
    m_RendererID = CreateProgram(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Reload()
{
    unsigned int rendererID = CreateProgram(m_VertexShaderPath, m_FragmentShaderPath);
    if (rendererID == 0)
    {
        std::cout << "Error: reloading shader program failed!" << std::endl;
        return;
    }
    glDeleteProgram(m_RendererID);
    m_RendererID = rendererID;
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

// private

std::string Shader::ParseShaderFile(const std::string& filepath)
{
    // C++ way of reading file, on the basis of how to do it with the C API (could be a little bit quicker)
    std::ifstream stream(filepath);
    std::string contents;
    stream.seekg(0, std::ios::end);
    contents.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&contents[0], contents.size());
    stream.close();
    return contents;
}

unsigned int Shader::CreateShader(const GLenum shaderType, const std::string& filepath)
{
    std::string fileStr = ParseShaderFile(filepath);
    const char* shaderSrc = fileStr.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    /* Error checking */
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, NULL, log);
        std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int Shader::CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderPath);
    unsigned int fragShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    unsigned int shaderProgram = glCreateProgram();
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
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteProgram(shaderProgram);
        return 0;
    }
    return shaderProgram;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations.at(name); // [name];
    
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform " << name << " not found!" << std::endl;

    m_UniformLocations.emplace(name, location); // [name] = location;
    return location;
}