//
// Created by root on 26/10/19.
//

#ifndef GRAPHICS_ASSIGNMENT_II_SHADING_H
#define GRAPHICS_ASSIGNMENT_II_SHADING_H

#include "glad/glad.h"
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>

typedef GLuint ShaderID;
typedef GLuint ProgramID;

class ShaderBase {
protected:
    ShaderID sid;
public:
    GLuint id();
};

template<GLenum T>
class Shader : public ShaderBase {
public:
    explicit Shader(const std::string &filename) : ShaderBase() {
        ShaderID shader = glCreateShader(T);

        std::ifstream file(filename);
        assert (file.is_open());

        std::string source(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

        file.close();

        const char *sourcePointer = source.c_str();
        glShaderSource(shader, 1, &sourcePointer, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        int infoLogLength;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (!compiled)
            if (infoLogLength > 0) {
                std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
                glGetShaderInfoLog(shader,
                                   infoLogLength,
                                   NULL,
                                   vertexShaderErrorMessage.data());
                std::cerr << &vertexShaderErrorMessage[0] << std::endl;

                assert(false);
            }

        sid = shader;
    }

    ~Shader() {
        glDeleteShader(sid);
    }
};

class Program {
    ProgramID pid;
public:
	Program();

    Program(std::initializer_list<ShaderBase> shaders);

    ProgramID id() { return pid; }

    void bind() const;

    ~Program();

};

#endif //GRAPHICS_ASSIGNMENT_II_SHADING_H
