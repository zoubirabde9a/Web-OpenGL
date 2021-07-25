#include "spritebatch.h"
#include <algorithm>
#include "stdio.h"

Sprite::Sprite() : m_texture(0), m_depth(0.0f), m_angle(0.0f) {

}
Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
    : m_destRect(destRect), m_uvRect(uvRect), m_texture(texture),  m_depth(depth), m_color(color), m_angle(0.0f) {

}
Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle)
    : m_destRect(destRect), m_uvRect(uvRect), m_texture(texture), m_depth(depth), m_color(color), m_angle(angle) {

}
Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir)
    : m_destRect(destRect), m_uvRect(uvRect), m_texture(texture), m_depth(depth), m_color(color) {
    const glm::vec2 right(1.0f, 0.0f);
    float angle = acos(glm::dot(right, dir));
    if (dir.y < 0.0f) angle = -angle;
    m_angle = angle;
}
Glyph::Glyph(const Sprite& sprite) :
    texture(sprite.getTexture()),
    depth(sprite.getDepth()), type(QUAD) {

    glm::vec2 halfDims(sprite.getDestRect().z / 2.0f, sprite.getDestRect().w / 2.0f);

    // Get points centered at origin
    glm::vec2 tl(-halfDims.x, halfDims.y);
    glm::vec2 bl(-halfDims.x, -halfDims.y);
    glm::vec2 br(halfDims.x, -halfDims.y);
    glm::vec2 tr(halfDims.x, halfDims.y);

    // Rotate the points
    tl = rotatePoint(tl, sprite.getAngle()) + halfDims;
    bl = rotatePoint(bl, sprite.getAngle()) + halfDims;
    br = rotatePoint(br, sprite.getAngle()) + halfDims;
    tr = rotatePoint(tr, sprite.getAngle()) + halfDims;

    topLeft.setColor(sprite.getColor());
    topLeft.setPosition(sprite.getDestRect().x + tl.x, sprite.getDestRect().y + tl.y);
    topLeft.setUV(sprite.getUvRect().x, sprite.getUvRect().y + sprite.getUvRect().w);

    bottomLeft.setColor(sprite.getColor());
    bottomLeft.setPosition(sprite.getDestRect().x + bl.x, sprite.getDestRect().y + bl.y);
    bottomLeft.setUV(sprite.getUvRect().x, sprite.getUvRect().y);

    bottomRight.setColor(sprite.getColor());
    bottomRight.setPosition(sprite.getDestRect().x + br.x, sprite.getDestRect().y + br.y);
    bottomRight.setUV(sprite.getUvRect().x + sprite.getUvRect().z, sprite.getUvRect().y);

    topRight.setColor(sprite.getColor());
    topRight.setPosition(sprite.getDestRect().x + tr.x, sprite.getDestRect().y + tr.y);
    topRight.setUV(sprite.getUvRect().x + sprite.getUvRect().z, sprite.getUvRect().y + sprite.getUvRect().w);
}
Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
    texture(Texture),
    depth(Depth), type(QUAD) {

    topLeft.setColor(color);
    topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.setColor(color);
    bottomLeft.setPosition(destRect.x, destRect.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.setColor(color);
    bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.setColor(color);
    topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
    texture(Texture),
    depth(Depth), type(QUAD) {

    glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

    // Get points centered at origin
    glm::vec2 tl(-halfDims.x, halfDims.y);
    glm::vec2 bl(-halfDims.x, -halfDims.y);
    glm::vec2 br(halfDims.x, -halfDims.y);
    glm::vec2 tr(halfDims.x, halfDims.y);

    // Rotate the points
    tl = rotatePoint(tl, angle) + halfDims;
    bl = rotatePoint(bl, angle) + halfDims;
    br = rotatePoint(br, angle) + halfDims;
    tr = rotatePoint(tr, angle) + halfDims;

    topLeft.setColor(color);
    topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.setColor(color);
    bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.setColor(color);
    bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.setColor(color);
    topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
Glyph::Glyph(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
    const Vertex& bottomRight, GLuint texture, float depth): texture(texture),
    depth(depth), topLeft(topLeft), bottomLeft(bottomLeft), topRight(topRight),
    bottomRight(bottomRight), type(QUAD){
}
Glyph::Glyph(const Vertex& v1, const Vertex& v2, const Vertex& v3, GLuint texture, float depth)
: texture(texture), depth(depth), v1(v1), v2(v2),
    v3(v3), type(TRIANGLE) {

}
Glyph::Glyph(Vertex* verts, size_t size, GLuint texture, float depth)
: texture(texture), depth(depth), verts(verts), vertsSize(size), type(ARRAY) {

}
glm::vec2 Glyph::rotatePoint(const glm::vec2& pos, float angle) {
    glm::vec2 newv;
    newv.x = pos.x * cos(angle) - pos.y * sin(angle);
    newv.y = pos.x * sin(angle) + pos.y * cos(angle);
    return newv;
}

SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)
{
}

SpriteBatch::~SpriteBatch()
{
    dispose();
}

void SpriteBatch::init() {
    createVertexArray();
}
void SpriteBatch::dispose() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

}
void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
    m_sortType = sortType;
}

void SpriteBatch::end() {
    // Set up all pointers for fast sorting
    m_glyphPointers.resize(m_glyphs.size());
    for (size_t i = 0; i < m_glyphs.size(); i++) {
        m_glyphPointers[i] = &m_glyphs[i];
    }

    //sortGlyphs();
    createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
    m_glyphs.push_back(Glyph(destRect, uvRect, texture, depth, color) );
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
    m_glyphs.push_back(Glyph(destRect, uvRect, texture, depth, color, angle));
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {
    const glm::vec2 right(1.0f, 0.0f);
    float angle = acos(glm::dot(right, dir));
    if (dir.y < 0.0f) angle = -angle;

    m_glyphs.push_back(Glyph(destRect, uvRect, texture, depth, color, angle));
}
void SpriteBatch::draw(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
    const Vertex& bottomRight, GLuint texture, float depth) {
    m_glyphs.push_back(Glyph(topLeft, bottomLeft, topRight, bottomRight, texture, depth));
}
void SpriteBatch::draw(const Vertex& v1, const Vertex& v2, const Vertex& v3, GLuint texture, float depth) {
    m_glyphs.push_back(Glyph(v1, v2, v3, texture, depth));
}
void SpriteBatch::draw(Vertex* verts, size_t vertsSize, GLuint texture, float depth) {
    m_glyphs.push_back(Glyph(verts, vertsSize, texture, depth));
}
void SpriteBatch::draw(const Sprite& sprite) {
    m_glyphs.push_back(Glyph(sprite));
}
void SpriteBatch::draw(const std::vector < Sprite > &sprites) {
        for (size_t i = 0; i < sprites.size(); i++) {
            m_glyphs.push_back(Glyph(sprites[i]));
        }
}
void SpriteBatch::draw(Sprite* sprites, size_t size) {
        for (size_t i = 0; i < size; i++) {
            m_glyphs.push_back(Glyph(sprites[i]));
        }
}
void SpriteBatch::renderBatch() {

    // Bind our VAO. This sets up the opengl state we need, including the
    // vertex attribute pointers and it binds the VBO

    glBindVertexArray(m_vao);
    for (size_t i = 0; i < m_renderBatches.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
        glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
    }

    glBindVertexArray(0);
    m_renderBatches.clear();

    // Makes _glpyhs.size() == 0, however it does not free internal memory.
    // So when we later call emplace_back it doesn't need to internally call new.
    m_glyphs.clear();
}

void SpriteBatch::createRenderBatches() {

    // This will store all the vertices that we need to upload
    std::vector <Vertex> vertices;
    // Resize the buffer to the exact size we need so we can treat
    // it like an array
    size_t verteciesSize = 0;
    for (auto &it : m_glyphPointers) {
        verteciesSize += 6 * (it->type == Glyph::QUAD) + 3 * (it->type == Glyph::TRIANGLE) + it->vertsSize * (it->type == Glyph::ARRAY);
    }
    vertices.resize(verteciesSize);

    if (m_glyphPointers.empty()) {
        return;
    }

    int offset = 0; // current offset
    int cv = 0; // current vertex

    //Add the first batch
    const Glyph & currentGlyph = *(m_glyphPointers[0]);
    GLuint lastTexture = currentGlyph.texture;
    if (currentGlyph.type == Glyph::QUAD) {
        m_renderBatches.emplace_back(offset, 6, currentGlyph.texture);

        vertices[cv++] = currentGlyph.topLeft;
        vertices[cv++] = currentGlyph.bottomLeft;
        vertices[cv++] = currentGlyph.bottomRight;
        vertices[cv++] = currentGlyph.bottomRight;
        vertices[cv++] = currentGlyph.topRight;
        vertices[cv++] = currentGlyph.topLeft;
        offset += 6;
    }
    else if (currentGlyph.type == Glyph::TRIANGLE) {
        m_renderBatches.emplace_back(offset, 3, currentGlyph.texture);

        vertices[cv++] = currentGlyph.v1;
        vertices[cv++] = currentGlyph.v2;
        vertices[cv++] = currentGlyph.v3;
        offset += 3;
    }
    else {
        m_renderBatches.emplace_back(offset, currentGlyph.vertsSize, currentGlyph.texture);
        for (size_t i = 0; i < currentGlyph.vertsSize; i++) {
            vertices[cv++] = currentGlyph.verts[i];
        }
        offset += currentGlyph.vertsSize;
    }

    //Add all the rest of the glyphs
    for (size_t cg = 1; cg < m_glyphPointers.size(); cg++) {
        const Glyph & glyph = *(m_glyphPointers[cg]);
        if (glyph.type == Glyph::QUAD) {
            // Check if this glyph can be part of the current batch
            if (glyph.texture != lastTexture) {
                // Make a new batch
                m_renderBatches.emplace_back(offset, 6, glyph.texture);
                lastTexture = glyph.texture;
            }
            else {
                // If its part of the current batch, just increase numVertices
               m_renderBatches.back().numVertices += 6;
            }
            vertices[cv++] = glyph.topLeft;
            vertices[cv++] = glyph.bottomLeft;
            vertices[cv++] = glyph.bottomRight;
            vertices[cv++] = glyph.bottomRight;
            vertices[cv++] = glyph.topRight;
            vertices[cv++] = glyph.topLeft;
            offset += 6;
        }
        else if (glyph.type == Glyph::TRIANGLE) {
            if (glyph.texture != lastTexture) {
                // Make a new batch
                m_renderBatches.emplace_back(offset, 3, glyph.texture);
                lastTexture = glyph.texture;
            }
            else {
                // If its part of the current batch, just increase numVertices
                m_renderBatches.back().numVertices += 3;
            }
            vertices[cv++] = glyph.v1;
            vertices[cv++] = glyph.v2;
            vertices[cv++] = glyph.v3;
            offset += 3;
        } else {
            if (glyph.texture != lastTexture) {
                // Make a new batch
                m_renderBatches.emplace_back(offset, glyph.vertsSize, glyph.texture);
                lastTexture = glyph.texture;
            }
            else {
                // If its part of the current batch, just increase numVertices
                m_renderBatches.back().numVertices += glyph.vertsSize;
            }
            for (size_t i = 0; i < glyph.vertsSize; i++) {
                vertices[cv++] = glyph.verts[i];
            }
            offset += glyph.vertsSize;
        }
    }

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Orphan the buffer (for speed)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertexArray() {

    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }
    glBindVertexArray(m_vao);

    //G enerate the VBO if it isn't already generated
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //Tell opengl what attribute arrays we need
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //This is the position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    //This is the color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, r));
    //This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

    glBindVertexArray(m_vao);

}

void SpriteBatch::sortGlyphs() {

    switch (m_sortType) {
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
            break;
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
            break;
         case GlyphSortType::NONE:

        break;
    }
}

bool SpriteBatch::compareFrontToBack(const Glyph* a, const Glyph* b) {
    return a->depth < b->depth;
}

bool SpriteBatch::compareBackToFront(const Glyph* a, const Glyph* b) {
    return a->depth > b->depth;
}

bool SpriteBatch::compareTexture(const Glyph * a, const Glyph * b) {
    return a->texture < b->texture;
}
void applyBoundry(glm::vec4 &uvs, glm::vec4 &destRect, const glm::vec4 &boundry) {
    float diff;
    float diffPercent;
    float tmp;
    if (destRect.x < boundry.x) {
        diff = boundry.x - destRect.x;
        diffPercent = diff / destRect.z;
        if (diffPercent < 1.0f && diffPercent > 0) {
            destRect.x = boundry.x;
            destRect.z -= destRect.z * diffPercent;
            tmp = diffPercent * uvs.z;
            uvs.z -= tmp;
            uvs.x += tmp;
        }
        else {
            destRect.z = 0;
            uvs.z = 0.0f;
        }
    }
    if (boundry.x + boundry.z < destRect.x + destRect.z) {
        diff = destRect.x + destRect.z - boundry.x - boundry.z;
        diffPercent = diff / destRect.z;
        if (diffPercent < 1.0f && diffPercent > 0) {
            destRect.z -= destRect.z * diffPercent;
            tmp = diffPercent * uvs.z;
            uvs.z -= tmp;
        }
        else {
            destRect.z = 0.0f;
            uvs.z = 0.0f;
        }
    }
    if (destRect.y < boundry.y) {
        diff = boundry.y - destRect.y;
        diffPercent = diff / destRect.w;
        if (diffPercent < 1.0f && diffPercent > 0) {
            destRect.y = boundry.y;
            destRect.w -= destRect.w * diffPercent;
            tmp = diffPercent * uvs.w;
            uvs.w -= tmp;
            uvs.y += tmp;
        }
        else {
            destRect.w = 0.0f;
            uvs.w = 0.0f;
        }
    }
    if (boundry.y + boundry.w < destRect.y + destRect.w) {
        diff = destRect.y + destRect.w - boundry.y - boundry.w;
        diffPercent = diff / destRect.w;
        if (diffPercent < 1.0f && diffPercent > 0) {
            destRect.w -= destRect.w * diffPercent;
            tmp = diffPercent * uvs.w;
            uvs.w -= tmp;
        }
        else {
            destRect.w = 0.0f;
            uvs.w = 0.0f;
        }
    }
}
