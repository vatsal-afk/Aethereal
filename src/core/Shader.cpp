#include "Shader.h"

#include <glad/glad.h>

#include <string>
#include <vector>
#include <iostream>

namespace Aethereal {

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
        Compile(vertexSrc, fragmentSrc);
    }

    Shader::~Shader() {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::SetUniform2f(const std::string& name, float v0, float v1) const {
        glUniform2f(GetUniformLocation(name), v0, v1);
    }

    int Shader::GetUniformLocation(const std::string& name) const {
        return glGetUniformLocation(m_RendererID, name.c_str());
    }

    void Shader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
        if (m_RendererID) {
            glDeleteProgram(m_RendererID);
        }

        m_VertexSource = vertexSrc;
        m_FragmentSource = fragmentSrc;

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        if (vertexShader == 0) return;

        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
        if (fragmentShader == 0) {
            glDeleteShader(vertexShader);
            return;
        }

        m_RendererID = LinkProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    std::string Shader::GetVertexSource() const {
        return m_VertexSource;
    }

    // -----------------------
    // Private helper methods
    // -----------------------

    GLuint Shader::CompileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);
        const GLchar* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, &log[0]);

            std::cerr << "Shader compilation failed:\n" << log.data() << std::endl;

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);
            glGetProgramInfoLog(program, logLength, &logLength, &log[0]);

            std::cerr << "Program linking failed:\n" << log.data() << std::endl;

            glDeleteProgram(program);
            return 0;
        }

        return program;
    }
}
