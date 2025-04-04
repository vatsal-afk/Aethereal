#pragma once

#include "Shader.h"

#include <string>
#include <cstdint>

namespace Aethereal {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        // uC and uK parameters
        void Render(float zoom, float offsetX, float offsetY, float uC = 0.0f, float uK = 0.0f);

        std::string GetVertexShader();
        std::string GenerateFragmentShader(const std::string& fx, const std::string& fy);
        
        void UpdateShader(const std::string& newFragmentSource);

        void SetFormula(const std::string& fx, const std::string& fy);

    private:
        uint32_t m_VAO, m_VBO;
        Shader* m_Shader;

        // initialize with Mandelbrot set
        std::string m_Fx = "x*x - y*y";
        std::string m_Fy = "2*x*y";
    };
}