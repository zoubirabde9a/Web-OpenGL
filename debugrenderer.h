#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H


#include <glm\glm.hpp>
#include <vector>
#include "glslprogram.h"
#include "vertex.h"

class DebugRenderer
{
public:
     struct DebugVertex {
        glm::vec2 position;
        ColorRGBA8 color;
    };
public:
    DebugRenderer();
    ~DebugRenderer();

    void init();
    void end();
    void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color);
    void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
    void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
    void drawShape(const float* points, size_t nbPoints, const ColorRGBA8& color, float angle);
    void render(const glm::mat4& projectionMatrix, float lineWidth);
    void destroy();

private:
    GLSLProgram m_program;
    std::vector < DebugVertex > m_verts;
    std::vector < GLuint > m_indices;
    GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
    int m_numElements = 0;
};



#endif // DEBUGRENDERER_H
