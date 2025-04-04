#pragma once

#include <glad/glad.h>

#include <string>
#include <cstdint>

namespace Aethereal {
    class Shader {
        public:
            Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
            ~Shader();

            void Bind() const; // it's a read-only access
            void Unbind() const;

            void SetUniform2f(const std::string& name, float v0, float v1) const;
            int GetUniformLocation(const std::string& name) const;
            
            void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);

            std::string GetVertexSource() const;

        private:
            uint32_t m_RendererID; // comes from <cstdint>

            // store the source of shaders
            std::string m_VertexSource;
            std::string m_FragmentSource;

            GLuint CompileShader(GLenum type, const std::string& source);
            GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
    };
}