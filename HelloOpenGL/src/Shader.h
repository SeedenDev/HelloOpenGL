#pragma once

#include <GL/glew.h>
#include <fstream>
#include <iostream>

class Shader
{
private:
    unsigned int m_RendererID;

private:
    std::string ShaderFileToString(const std::string& filepath)
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

    unsigned int CreateShader(const GLenum shaderType, const std::string& filepath)
    {
        std::string fileStr = ShaderFileToString(filepath);
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
            char* log = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(shader, length, NULL, log);
            std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << log << std::endl;
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

    int GetUniformLocation(const char* name) const;
	void Bind() const;
	void Unbind() const;
};