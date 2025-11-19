#include "Shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, "assets/shaders/vertex.glsl");
    unsigned int fragShader = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/fragment.glsl");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

	//TODO: better error handling (including how to notice the failure from where it has been called)
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        m_RendererID = 0;
        return;
    }

    m_RendererID = shaderProgram;
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

int Shader::GetUniformLocation(const char* name) const
{
    return glGetUniformLocation(m_RendererID, name);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}