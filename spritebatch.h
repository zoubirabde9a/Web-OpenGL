#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include "opengl.h"

enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};
class Sprite {
public:
    Sprite();
    Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
    Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
    Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

    inline const glm::vec4& getDestRect() const { return m_destRect; }
    inline const glm::vec4& getUvRect() const { return m_uvRect; }
    inline GLuint getTexture() const { return m_texture; }
    inline float getDepth() const { return m_depth; }
    inline const ColorRGBA8 &getColor() const { return m_color; }
    inline float getAngle() const { return m_angle; }
private:
    glm::vec4 m_destRect;
    glm::vec4 m_uvRect;
    GLuint m_texture;
    float m_depth;
    ColorRGBA8 m_color;
    float m_angle;
};

class Glyph {
public:
    enum Type {
        QUAD, TRIANGLE, ARRAY
    };
public:
    Glyph() {}
    Glyph(const Sprite& sprite);
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);
    Glyph(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
        const Vertex& bottomRight, GLuint texture, float depth);
    Glyph(const Vertex& v1, const Vertex& v2, const Vertex& v3, GLuint texture, float depth);
    Glyph(Vertex* verts, size_t size, GLuint texture, float depth);

    GLuint texture;
    float depth;
    Type type;
    union {
        struct {
            Vertex topLeft;
            Vertex bottomLeft;
            Vertex topRight;
            Vertex bottomRight;
        };
        struct {
            Vertex v1, v2, v3;
        };
        struct {
            Vertex* verts;
            size_t vertsSize;
        };
    };
private:
    glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
};
class RenderBatch {
public:
    RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
        numVertices(NumVertices), texture(Texture) {
    }
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    void init();
    void dispose();

    void begin(GlyphSortType sortType = GlyphSortType::NONE);
    void end();

    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

    void draw(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
        const Vertex& bottomRight, GLuint texture, float depth);
    void draw(const Vertex& v1, const Vertex& v2, const Vertex& v3, GLuint texture, float depth);
    void draw(Vertex* verts, size_t vertsSize, GLuint texture, float depth);

    void draw(const Sprite& glyph);
    void draw(const std::vector < Sprite > &sprites);
    void draw(Sprite* sprites, size_t size);
    void renderBatch();

private:
    void createRenderBatches();
    void createVertexArray();
    void sortGlyphs();
    static bool compareFrontToBack(const Glyph* a, const Glyph* b);
    static bool compareBackToFront(const Glyph* a, const Glyph* b);
    static bool compareTexture(const Glyph* a, const Glyph* b);
private:
    GLuint m_vbo;
    GLuint m_vao;
    GlyphSortType m_sortType;
    std::vector< Glyph* > m_glyphPointers; ///< This is for sorting
    std::vector< Glyph  > m_glyphs; ///< These are the actual glyphs
    std::vector<RenderBatch> m_renderBatches;
};

extern void applyBoundry(glm::vec4 &uvs, glm::vec4 &destRect, const glm::vec4 &boundry);
#endif // SPRITEBATCH_H
