#pragma once

#include <string>
#include <glbinding/gl/gl.h>

enum ShaderType
{
    VERTEX = 0,
    FRAGMENT = 1,
    UNDEFINED = 2,
};

enum ShaderStatus
{
    UNINICIALIZED,
    READING_ERROR,
    COMPILE_ERROR,
    SUCCESS,
};

template <ShaderType T>
class Shader : public ShaderBase
{
public:
    Shader(const std::string& path) : ShaderBase(path, T) {}

    Shader(const Shader& oldShader) : shader(oldShader.shader), type(oldShader.type), status(oldShader.status) {};
};

class ShaderBase
{
public:
    gl::GLuint shader;
    ShaderType type;
    ShaderStatus status = UNINICIALIZED;

    ~ShaderBase() {
        gl::glDeleteShader(shader);
    }

    ShaderBase(ShaderBase&& srcShader) : shader(NULL), type(UNDEFINED), status(UNINICIALIZED)
    {
        shader = srcShader.shader;
        type = srcShader.type;
        status = srcShader.status;

        srcShader.shader = NULL;
        srcShader.type = UNDEFINED;
        srcShader.status = UNINICIALIZED;
    }

    ShaderBase& operator=(ShaderBase&& srcShader) noexcept
    {
        if (this != &srcShader)
        {
            shader = 0;

            shader = srcShader.shader;
            type = srcShader.type;
            status = srcShader.status;

            srcShader.shader = NULL;
            srcShader.type = UNDEFINED;
            srcShader.status = UNINICIALIZED;
        }

        return *this;
    }

private:
    template <ShaderType shaderType>
    friend class Shader;

    inline static gl::GLenum shaderTypeToGl[] = { gl::GL_VERTEX_SHADER, gl::GL_FRAGMENT_SHADER };

    ShaderBase(const std::string& path, ShaderType currType) : type(currType)
    {
        const gl::GLchar* shaderCode;
        std::string code = readFromFile(path);
        shaderCode = code.c_str();
        Compile(shaderCode);
        printLog();
    }

    std::vector<gl::GLchar> infolog;
    std::string readFromFile(const std::string& path);
    bool Compile(const gl::GLchar* shaderCode);
    void printLog();
};

class ShaderProgram {
public:
    ShaderProgram(Shader<VERTEX>&& vShader, Shader<FRAGMENT>&& fShader);

    void printLog();

    void Use()
    {
        gl::glUseProgram(program);
    }
    gl::GLuint getProgram()
    {
        return program;
    }

private:
    gl::GLuint program;
    std::vector<gl::GLchar> infolog;
    std::vector<ShaderBase> shaders;
};
