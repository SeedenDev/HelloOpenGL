#pragma once

#include <GL/glew.h>
#include <iostream>

#include "FileUtils.h"

class Shader {
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	int GetUniformLocation(const char* name) const;

private:
	unsigned int m_RendererID;

private:
    unsigned int CreateShader(const GLenum shaderType, const std::string& filepath)
    {
        std::string fileStr = ReadFileAsString(filepath);
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
};