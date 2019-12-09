//
// Created by root on 26/10/19.
//

#include "shading.h"

GLuint ShaderBase::id() {
    return sid;
}

Program::Program() : pid(-1){}

Program::Program(std::initializer_list<ShaderBase> shaders) {
    ProgramID program = glCreateProgram();

    for (auto shader : shaders)
        glAttachShader(program, shader.id());

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);

        std::cerr << &infoLog[0] << std::endl;
        assert(false);
    }
    pid = program;
}

void Program::bind() const
{
    glUseProgram(pid);
}

Program::~Program() { 
	glDeleteProgram(pid); }
