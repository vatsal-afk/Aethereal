#include "Renderer.h"

#include <glad/glad.h>

namespace Aethereal {
    Renderer::Renderer() {

        float vertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f,
            -1.0f,  1.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        m_Shader = new Shader(GetVertexShader(), GenerateFragmentShader(m_Fx, m_Fy));
    }

    Renderer::~Renderer() {
        delete m_Shader;
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Renderer::Render(float zoom, float offsetX, float offsetY, float uC, float uK) {
        m_Shader->Bind();
        
        // Set zoom and offset uniforms
        m_Shader->SetUniform2f("uZoomOffset", zoom, 0.0f);
        m_Shader->SetUniform2f("uOffset", offsetX, offsetY);
        
        // Set the uC and uK parameters that are used in the fragment shader
        m_Shader->SetUniform2f("uC", uC, 0.0f);
        m_Shader->SetUniform2f("uK", uK, 0.0f);
        
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_Shader->Unbind();
    }

    std::string Renderer::GetVertexShader() {
        return R"(
            #version 330 core
            layout(location = 0) in vec2 aPos;
            void main() {
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";
    }

    std::string Renderer::GenerateFragmentShader(const std::string& fx, const std::string& fy) {
        return R"(
            #version 330 core
            out vec4 FragColor;
            
            // Define all needed uniforms
            uniform vec2 uZoomOffset;  // Zoom factor
            uniform vec2 uOffset;      // View offset
            uniform vec2 uC;           // Parameter C for the formula
            uniform vec2 uK;           // Parameter K for the formula
    
            vec3 hsv2rgb(vec3 c) {
                vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
                vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
                return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
            }
    
            void main() {
                // Apply zoom and offset to coordinates
                float x = (gl_FragCoord.x / 800.0 - 0.5) * 4.0 / uZoomOffset.x + uOffset.x;
                float y = (gl_FragCoord.y / 600.0 - 0.5) * 4.0 / uZoomOffset.x + uOffset.y;
    
                int maxIter = 100;
                int i;
                float smooth_i = 0.0;
    
                for (i = 0; i < maxIter; i++) {
                    float newX = )" + fx + R"(;
                    float newY = )" + fy + R"(;
                    
                    // Use the uC and uK parameters in the iteration
                    x = newX + uC.x;
                    y = newY + uK.x;
    
                    if (x*x + y*y > 4.0) {
                        smooth_i = float(i) + 1.0 - log(log(sqrt(x*x + y*y))) / log(2.0);
                        break;
                    }
                }
    
                if (i == maxIter) {
                    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
                } else {
                    float normalizedI = smooth_i / float(maxIter);
                    vec3 hsv = vec3(
                        0.5 + 0.5 * sin(normalizedI * 10.0),
                        0.8,
                        mix(0.6, 1.0, normalizedI)
                    );
                    vec3 rgb = hsv2rgb(hsv);
                    FragColor = vec4(rgb, 1.0);
                }
            }
        )";
    }
    
    void Renderer::UpdateShader(const std::string& newFragmentSource) {
        if (m_Shader) {
            m_Shader->Compile(m_Shader->GetVertexSource(), newFragmentSource);
        }
    }

    void Renderer::SetFormula(const std::string& fx, const std::string& fy) {
        m_Fx = fx;
        m_Fy = fy;
        std::string newFrag = GenerateFragmentShader(m_Fx, m_Fy);
        UpdateShader(newFrag);
    }    
}
