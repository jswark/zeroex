#include "shaderSystem/shaders.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string ShaderBase::readFromFile(const std::string& path) {
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);

    try
    {
        file.open(path);
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        status == READING_ERROR;
    }

    return code;
}

bool ShaderBase::Compile(const gl::GLchar* shaderCode) {
    shader = gl::glCreateShader(shaderTypeToGl[type]);
    gl::glShaderSource(shader, 1, &shaderCode, NULL);
    gl::glCompileShader(shader);

    gl::GLint isLinked = 0;
    gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &isLinked);
    // Print linking errors if any
    if (!isLinked)
    {
        gl::GLint maxLen = 0;
        gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &maxLen);
        infolog.resize(maxLen + 1);

        gl::glGetShaderInfoLog(shader, maxLen, &maxLen, &infolog[0]);
        printLog();

        status = COMPILE_ERROR;
        return isLinked;
    }

    status = SUCCESS;
    return isLinked;
}

void ShaderBase::printLog() {
    if (status == UNINICIALIZED) {
        std::cerr << "ERROR::SHADER::" << type << "::UNINICIALIZED" << std::endl;
    }
    else if (status == READING_ERROR) {
        std::cerr << "ERROR::SHADER::" << type << "::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    else if (status == COMPILE_ERROR) {
        std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << std::endl;

        for (uint32_t i = 0; i < infolog.size(); ++i) {
            std::cerr << infolog[i];
        }

        std::cout << std::endl;
    }
    else if (status == SUCCESS) {
        std::cerr << "SHADER::" << type << "::COMPILE_SUCCESSFULL" << std::endl;
    }
}

ShaderProgram::ShaderProgram(Shader<VERTEX>&& vShader, Shader<FRAGMENT>&& fShader) : program(gl::glCreateProgram())
{
    if (!program)
    {
        throw "Program was not initialized";
        return;
    }
    // Attach shaders to the program
    gl::glAttachShader(program, vShader.shader);
    gl::glAttachShader(program, fShader.shader);

    // Link program
    gl::GLint isLinked = 0;
    gl::glLinkProgram(program);
    gl::glGetProgramiv(program, gl::GL_LINK_STATUS, &isLinked);

    // Print linking errors if any
    if (!isLinked)
    {
        gl::GLint maxLen = 0;
        gl::glGetProgramiv(program, gl::GL_INFO_LOG_LENGTH, &maxLen);
        infolog.resize(maxLen);

        gl::glGetProgramInfoLog(program, maxLen, &maxLen, &infolog[0]);
        printLog();

        return;
    }

    shaders.push_back(std::move(vShader));
    shaders.push_back(std::move(fShader));
}

void ShaderProgram::printLog() {
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
    for (uint32_t i = 0; i < infolog.size(); ++i) {
        std::cerr << infolog[i];
    }
    std::cout << std::endl;
}
