#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../graph/graph.hpp"
#include "../config/config.hpp"
#include "../core/logger.hpp"

namespace plot_genius {
namespace rendering {

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void RenderGraph(const std::vector<Point>& points, 
                    double xMin, double xMax,
                    const config::Config::GraphSettings& settings);
    
    void RenderGrid(double xMin, double xMax, double yMin, double yMax);
    void RenderAxes(double xMin, double xMax, double yMin, double yMax);
    
    void SetViewport(int width, int height);
    void Clear();

private:
    struct ShaderProgram {
        GLuint id;
        GLuint vertexShader;
        GLuint fragmentShader;
    };
    
    struct Buffer {
        GLuint vao;
        GLuint vbo;
    };
    
    bool CompileShader(const char* source, GLenum type, GLuint& shader);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint& program);
    
    ShaderProgram m_graphShader;
    ShaderProgram m_gridShader;
    
    Buffer m_graphBuffer;
    Buffer m_gridBuffer;
    
    glm::mat4 m_projection;
    glm::mat4 m_view;
    
    int m_gridVertexCount;
    
    bool SetupShaders();
    void CreateGraphBuffers();
    void CreateGridBuffers();
    void UpdateMatrices();
};

} // namespace rendering
} // namespace plot_genius 