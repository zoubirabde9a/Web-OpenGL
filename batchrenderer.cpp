#include "batchrenderer.h"
#include <algorithm>
#include <string.h>


bool compareFrontToBack(Batch* a, Batch* b) {
    return (a->depth < b->depth);
}

bool compareBackToFront(Batch* a, Batch* b) {
    return (a->depth > b->depth);
}
bool compareProgram(Batch* a, Batch* b) {
    return (a->program > b->program);
}
// Sprite
Batch::Batch(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, GLSLProgram* program, const ColorRGBA8& color, float angle) {
    s_x = destRect.x; s_y = destRect.y; s_z = destRect.z; s_w = destRect.w;
    s_uvx = uvRect.x; s_uvy = uvRect.y; s_u = uvRect.z; s_v = uvRect.w;
    s_r = color.r; s_g = color.g; s_b = color.b; s_a = color.a;
    s_texture = texture;
    this->depth = depth;
    this->program = program;
    s_angle = angle;
    type = SPRITE;
}
// Sprites
Batch::Batch(const std::vector < Sprite >& sprites, GLSLProgram* program) {
    this->program = program;
    ss_size = sprites.size();
    ss_sprites = new Sprite[sprites.size()];
    for (size_t i = 0; i < sprites.size(); i++) {
        ss_sprites[i] = sprites[i];
    }
    if (sprites.size() > 0) {
        this->depth = sprites[0].getDepth();
    }
    type = SPRITES;
}
// Quad
Batch::Batch(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
    const Vertex& bottomRight, GLuint texture, float depth, GLSLProgram* program) {
    this->program = program;
    this->depth = depth;
    q_topLeft = topLeft;
    q_bottomLeft = bottomLeft;
    q_topRight = topRight;
    q_bottomRight = bottomRight;
    q_texture = texture;
    type = QUAD;
}
// Vertices
Batch::Batch(Vertex* verts, size_t vertsSize, GLuint texture, float depth, GLSLProgram* program) {
    this->program = program;
    this->depth = depth;
    v_verts = verts;
    v_vertsSize = vertsSize;
    v_texture = texture;
    type = VERTICES;
}
// Light
Batch::Batch(const glm::vec2 &position, float radius, GLuint texture, float depth, GLSLProgram* program, const ColorRGBA8& color) {
    this->program = program;
    this->depth = depth;
    l_x = position.x;
    l_y = position.y;
    l_radius = radius;
    l_texture = texture;
    l_r = color.r;
    l_g = color.g;
    l_b = color.b;
    l_a = color.a;
    type = LIGHT;
}
// Shape
Batch::Batch(const float* points, size_t nbPoints, float depth, const ColorRGBA8& color) {
    sh_points = points;
    sh_nbPoints = nbPoints;
    sh_r = color.r; sh_g = color.g; sh_b = color.b; sh_a = color.a;
    this->depth = depth;
    type = SHAPE;
}
// Rectangle
Batch::Batch(const glm::vec4 &destRect, float depth, const ColorRGBA8& color, float angle) {
    r_x = destRect.x; r_y = destRect.y; r_z = destRect.z; r_w = destRect.w;
    r_r = color.r; r_g = color.g; r_b = color.b; r_a = color.a;
    this->depth = depth;
    r_angle = angle;
    type = RECTANGLE;
}
// Circle
Batch::Batch(const glm::vec2 &position, float radius, float depth, const ColorRGBA8& color) {
    c_x = position.x; c_y = position.y;
    c_radius = radius;
    c_r = color.r; c_g = color.g; c_b = color.b; c_a = color.a;
    this->depth = depth;
    type = CIRCLE;
}
// Font
Batch::Batch(SpriteFont* font, const glm::vec2 &position, const std::string &text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just) {
    this->program = program;
    this->depth = depth;
    f_x = position.x; f_y = position.y;
    f_sx = scalling.x;
    f_sy = scalling.y;
    f_r = color.r;
    f_g = color.g;
    f_b = color.b;
    f_a = color.a;
    f_text = new char[text.size() + 1];
    strcpy(f_text, text.c_str());
    f_font = font;
    f_just = just;
    type = FONT;
}
// Font Boundry
Batch::Batch(SpriteFont* font, const glm::vec2 &position, const std::string &text, const glm::vec2 &scalling, float depth, const glm::vec4& boundry, GLSLProgram* program, const ColorRGBA8& color, Justification just) {
    this->program = program;
    this->depth = depth;
    fb_x = position.x; fb_y = position.y;
    fb_sx = scalling.x;
    fb_sy = scalling.y;
    fb_r = color.r;
    fb_g = color.g;
    fb_b = color.b;
    fb_a = color.a;
    fb_text = new char[text.size() + 1];
    strcpy(fb_text, text.c_str());
    fb_font = font;
    fb_just = just;
    fb_bx = boundry.x;
    fb_by = boundry.y;
    fb_bz = boundry.z;
    fb_bw = boundry.w;
    type = FONT_BOUNDRY;
}
// Color Font
Batch::Batch(SpriteFont* font, const glm::vec2 &position, const TextRGB8 &text, const glm::vec2 &scalling, float depth, GLSLProgram* program, Justification just, GLubyte alpha) {
    this->program = program;
    this->depth = depth;
    cf_x = position.x; cf_y = position.y;
    cf_sx = scalling.x;
    cf_sy = scalling.y;
    cf_colorText = new TextRGB8(text);
    cf_font = font;
    cf_just = just;
    cf_alpha = alpha;
    type = COLOR_FONT;
}
// Color Font Boundry
Batch::Batch(SpriteFont* font, const glm::vec2 &position, const TextRGB8 &text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, Justification just, GLubyte alpha) {
    this->program = program;
    this->depth = depth;
    cfb_x = position.x; cfb_y = position.y;
    cfb_sx = scalling.x;
    cfb_sy = scalling.y;
    cfb_colorText = new TextRGB8(text);
    cfb_font = font;
    cfb_just = just;
    cfb_alpha = alpha;
    cfb_bx = boundry.x;
    cfb_by = boundry.y;
    cfb_bz = boundry.z;
    cfb_bw = boundry.w;
    type = COLOR_FONT_BOUNDRY;
}
/*glm::vec4 Batch::getDestRect() const {
    return glm::vec4(x, y, z, w);
}
glm::vec2 Batch::getPosition() const {
    return glm::vec2(x, y);
}
glm::vec2 Batch::getScalling() const {
    return glm::vec2(sx, sy);
}
ColorRGBA8 Batch::getColor() const {
    return ColorRGBA8(r, g, b, a);
}
GLSLProgram* Batch::getProgram() {
    return program;
}
glm::vec4 Batch::getUvs() const {
    return glm::vec4(uvx, uvy, u, v);
}
GLuint Batch::getTexture() const {
    return texture;
}
float Batch::getAngle() const {
    return angle;
}
float Batch::getRadius() const {
    return radius;
}
*/
void BatchRenderer::init() {
    m_spriteBatch.init();
    m_debugRenderer.init();
}
void BatchRenderer::dispose() {
    m_spriteBatch.dispose();
    m_debugRenderer.destroy();
}
void BatchRenderer::begin(BatchSortType sortType) {
    m_sortType = sortType;
}
void BatchRenderer::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, GLSLProgram* prog, const ColorRGBA8& color, float angle) {
    m_batches.emplace_back(destRect, uvRect, texture, depth, prog, color, angle);
}
void BatchRenderer::draw(Vertex* verts, size_t vertsSize, GLuint texture, float depth, GLSLProgram* program) {
    m_batches.emplace_back(verts, vertsSize, texture, depth, program);
}
void BatchRenderer::draw(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
    const Vertex& bottomRight, GLuint texture, float depth, GLSLProgram* program) {
    m_batches.emplace_back(topLeft, bottomLeft, topRight, bottomRight, texture, depth, program);
}
void BatchRenderer::drawSprites(const std::vector < Sprite > &sprites, GLSLProgram* program) {
    m_batches.emplace_back(sprites, program);
}
void BatchRenderer::drawLight(const glm::vec2 &position, float radius, GLuint texture, float depth, GLSLProgram* prog, const ColorRGBA8 &color) {
    m_batches.emplace_back(position, radius, texture, depth, prog, color);
}
void BatchRenderer::drawShape(const float* points, size_t nbPoints, float depth, const ColorRGBA8& color) {
    m_batches.emplace_back(points, nbPoints, depth, color);
}
void BatchRenderer::drawRectangle(const glm::vec4& rect, float depth, const ColorRGBA8& color, float angle) {
    m_batches.emplace_back(rect, depth, color, angle);
}
void BatchRenderer::drawCircle(const glm::vec2 &position, float radius, float depth, const ColorRGBA8& color) {
    m_batches.emplace_back(position, radius, depth, color);
}
void BatchRenderer::drawText(SpriteFont* font, const glm::vec2 &position, const char* text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just) {
    m_batches.emplace_back(font, position, text, scalling, depth, program, color, just);
}
void BatchRenderer::drawText(SpriteFont* font, const glm::vec2 &position, const std::string& text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just) {
    m_batches.emplace_back(font, position, text, scalling, depth, program, color, just);
}
void BatchRenderer::drawText(SpriteFont* font, const glm::vec2 &position, const std::string& text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, const ColorRGBA8& color, Justification just) {
    m_batches.emplace_back(font, position, text, scalling, depth, boundry, program, color, just);
}
void BatchRenderer::drawText(SpriteFont* font, const glm::vec2 &position, const TextRGB8& text, const glm::vec2 &scalling, float depth, GLSLProgram* program, Justification just, GLubyte alpha) {
    m_batches.emplace_back(font, position, text, scalling, depth, program, just, alpha);
}
void BatchRenderer::drawText(SpriteFont* font, const glm::vec2 &position, const TextRGB8& text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, Justification just, GLubyte alpha) {
    m_batches.emplace_back(font, position, text, scalling, depth, boundry, program, just, alpha);
}
void BatchRenderer::render(const glm::mat4& projectionMat) {
    if (m_batches.size() == 0)return;
    createBatchPtrs();
    sortBatchPtrs();
    GLSLProgram* program = nullptr;
    GLint pUniform = 0;
    if (m_batchPtrs[0]->type == Batch::Type::SPRITE || m_batchPtrs[0]->type == Batch::Type::LIGHT || m_batchPtrs[0]->type == Batch::Type::FONT || m_batchPtrs[0]->type == Batch::Type::FONT_BOUNDRY || m_batchPtrs[0]->type == Batch::Type::COLOR_FONT || m_batchPtrs[0]->type == Batch::Type::COLOR_FONT_BOUNDRY) {
        program = m_batchPtrs[0]->program;
        program->use();
        pUniform = program->getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
        m_spriteBatch.begin();
    }
    for (auto &it : m_batchPtrs) {
        if (it->type == Batch::Type::SPRITE) {
            glm::vec4 destRect(it->s_x, it->s_y, it->s_z, it->s_w);
            glm::vec4 uvs(it->s_uvx, it->s_uvy, it->s_u, it->s_v);
            ColorRGBA8 color(it->s_r, it->s_g, it->s_b, it->s_a);
            if (program) {
                if (it->program == program) {
                    m_spriteBatch.draw(destRect, uvs, it->s_texture, it->depth, color, it->s_angle);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    m_spriteBatch.draw(destRect, uvs, it->s_texture, it->depth, color, it->s_angle);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                m_spriteBatch.draw(destRect, uvs, it->s_texture, it->depth, color, it->s_angle);
            }
        }
        else if (it->type == Batch::Type::SPRITES) {
            if (program) {
                if (it->program == program) {
                    m_spriteBatch.draw(it->ss_sprites, it->ss_size);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    m_spriteBatch.draw(it->ss_sprites, it->ss_size);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                m_spriteBatch.draw(it->ss_sprites, it->ss_size);
            }
        }
        else if (it->type == Batch::Type::QUAD) {
            if (program) {
                if (it->program == program) {
                    m_spriteBatch.draw(it->q_topLeft, it->q_bottomLeft, it->q_topRight, it->q_bottomRight, it->q_texture, it->depth);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    m_spriteBatch.draw(it->q_topLeft, it->q_bottomLeft, it->q_topRight, it->q_bottomRight, it->q_texture, it->depth);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                m_spriteBatch.draw(it->q_topLeft, it->q_bottomLeft, it->q_topRight, it->q_bottomRight, it->q_texture, it->depth);
            }
        }
        else if (it->type == Batch::Type::VERTICES) {
            if (program) {
                if (it->program == program) {
                    m_spriteBatch.draw(it->v_verts, it->v_vertsSize, it->v_texture, it->depth);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    m_spriteBatch.draw(it->v_verts, it->v_vertsSize, it->v_texture, it->depth);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                m_spriteBatch.draw(it->v_verts, it->v_vertsSize, it->v_texture, it->depth);
            }
        }
        else if (it->type == Batch::Type::LIGHT) {
            glm::vec4 destRect(it->l_x - it->l_radius, it->l_y - it->l_radius, it->l_radius * 2.0f, it->l_radius * 2.0f);
            glm::vec4 uvs(-1, -1, 2, 2);
            ColorRGBA8 color(it->l_r, it->l_g, it->l_b, it->l_a);
            if (program) {
                if (it->program == program) {
                    m_spriteBatch.draw(destRect, uvs, it->l_texture, 0, color);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    m_spriteBatch.draw(destRect, uvs, it->l_texture, 0, color);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                m_spriteBatch.draw(destRect, uvs, it->l_texture, 0, color);
            }
        }
        else if (it->type == Batch::Type::FONT) {
            glm::vec2 position(it->f_x, it->f_y);
            glm::vec2 scalling(it->f_sx, it->f_sy);
            ColorRGBA8 color(it->f_r, it->f_g, it->f_b, it->f_a);
            if (program) {
                if (it->program == program) {
                    it->f_font->draw(m_spriteBatch, it->f_text, position, scalling, it->depth, color, it->f_just);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                   it->f_font->draw(m_spriteBatch, it->f_text, position, scalling, it->depth, color, it->f_just);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                it->f_font->draw(m_spriteBatch, it->f_text, position, scalling, it->depth, color, it->f_just);
            }
        }
        else if (it->type == Batch::Type::FONT_BOUNDRY) {
            glm::vec2 position(it->fb_x, it->fb_y);
            glm::vec2 scalling(it->fb_sx, it->fb_sy);
            glm::vec4 boundry(it->fb_bx, it->fb_by, it->fb_bz, it->fb_bw);
            ColorRGBA8 color(it->fb_r, it->fb_g, it->fb_b, it->fb_a);
            if (program) {
                if (it->program == program) {
                    it->fb_font->draw(m_spriteBatch, it->fb_text, position, scalling, it->depth, boundry, color, it->fb_just);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    it->fb_font->draw(m_spriteBatch, it->fb_text, position, scalling, it->depth, boundry, color, it->fb_just);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                it->fb_font->draw(m_spriteBatch, it->fb_text, position, scalling, it->depth, boundry, color, it->fb_just);
            }
        }
        else if (it->type == Batch::Type::COLOR_FONT) {
            glm::vec2 position(it->cf_x, it->cf_y);
            glm::vec2 scalling(it->cf_sx, it->cf_sy);
            if (program) {
                if (it->program == program) {
                    it->cf_font->draw(m_spriteBatch, *it->cf_colorText, position, scalling, it->depth, it->cf_just, it->cf_alpha);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    it->cf_font->draw(m_spriteBatch, *it->cf_colorText, position, scalling, it->depth, it->cf_just, it->cf_alpha);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                it->cf_font->draw(m_spriteBatch, *it->cf_colorText, position, scalling, it->depth, it->cf_just, it->cf_alpha);
            }
        }
        else if (it->type == Batch::Type::COLOR_FONT_BOUNDRY) {
            glm::vec2 position(it->cfb_x, it->cfb_y);
            glm::vec2 scalling(it->cfb_sx, it->cfb_sy);
            glm::vec4 boundry(it->cfb_bx, it->cfb_by, it->cfb_bz, it->cfb_bw);
            if (program) {
                if (it->program == program) {
                    it->cfb_font->draw(m_spriteBatch, *it->cfb_colorText, position, scalling, it->depth, boundry, it->cfb_just, it->cfb_alpha);
                }
                else {
                    m_spriteBatch.end();
                    m_spriteBatch.renderBatch();
                    program->unuse();
                    program = it->program;
                    program->use();
                    pUniform = program->getUniformLocation("P");
                    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                    m_spriteBatch.begin();
                    it->cfb_font->draw(m_spriteBatch, *it->cfb_colorText, position, scalling, it->depth, boundry, it->cfb_just, it->cfb_alpha);
                }
            }
            else {
                m_debugRenderer.end();
                m_debugRenderer.render(projectionMat, 2.0f);
                program = it->program;
                program->use();
                pUniform = program->getUniformLocation("P");
                glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMat[0][0]);
                m_spriteBatch.begin();
                it->cfb_font->draw(m_spriteBatch, *it->cfb_colorText, position, scalling, it->depth, boundry, it->cfb_just, it->cfb_alpha);
            }
        }
        else if (it->type == Batch::Type::RECTANGLE) {
            glm::vec4 destRect(it->r_x, it->r_y, it->r_z, it->r_w);
            ColorRGBA8 color(it->r_r, it->r_g, it->r_b, it->r_a);
            if (program) {
                m_spriteBatch.end();
                m_spriteBatch.renderBatch();
                program->unuse();
                program = nullptr;
                m_debugRenderer.drawBox(destRect, color, it->r_angle);
            }
            else {
                m_debugRenderer.drawBox(destRect, color, it->r_angle);
            }
        }
        else if (it->type == Batch::Type::CIRCLE) {
            glm::vec2 position(it->c_x, it->c_y);
            ColorRGBA8 color(it->c_r, it->c_g, it->c_b, it->c_a);
            if (program) {
                m_spriteBatch.end();
                m_spriteBatch.renderBatch();
                program->unuse();
                program = nullptr;
                m_debugRenderer.drawCircle(position, color, it->c_radius);
            }
            else {
                m_debugRenderer.drawCircle(position, color, it->c_radius);
            }
        }
        else if (it->type == Batch::Type::SHAPE) {
            ColorRGBA8 color(it->sh_r, it->sh_g, it->sh_b, it->sh_a);
            if (program) {
                m_spriteBatch.end();
                m_spriteBatch.renderBatch();
                program->unuse();
                program = nullptr;
                m_debugRenderer.drawShape(it->sh_points, it->sh_nbPoints, color, 0.0f);
            }
            else {
                m_debugRenderer.drawShape(it->sh_points, it->sh_nbPoints, color, 0.0f);
            }
        }
    }
    if (program) {
        m_spriteBatch.end();
        m_spriteBatch.renderBatch();
        program->unuse();
    }
    else {
        m_debugRenderer.end();
        m_debugRenderer.render(projectionMat, 2);
    }
    for (auto &it : m_batches) {
        if (it.type == Batch::Type::FONT) {
            delete[] it.f_text;
        }
        else if (it.type == Batch::Type::FONT_BOUNDRY){
            delete[] it.fb_text;
        }
        else if (it.type == Batch::Type::COLOR_FONT) {
            delete it.cf_colorText;
        }
        else if (it.type == Batch::Type::COLOR_FONT_BOUNDRY){
            delete it.cfb_colorText;
        }
        else if (it.type == Batch::Type::SPRITES) {
            delete[] it.ss_sprites;
        }
    }
    m_batches.clear();
}
void BatchRenderer::createBatchPtrs() {
    m_batchPtrs.resize(m_batches.size());
    for (size_t i = 0; i < m_batches.size(); i++) {
        m_batchPtrs[i] = &m_batches[i];
    }
}
void BatchRenderer::sortBatchPtrs() {
    if (m_sortType == BatchSortType::FRONT_TO_BACK) {
        std::stable_sort(m_batchPtrs.begin(), m_batchPtrs.end(), compareFrontToBack);
    }
    else if (m_sortType == BatchSortType::BACK_TO_FRONT) {
        std::stable_sort(m_batchPtrs.begin(), m_batchPtrs.end(), compareBackToFront);
    }
    else if (m_sortType == BatchSortType::PROGRAM) {
        std::stable_sort(m_batchPtrs.begin(), m_batchPtrs.end(), compareProgram);
    }
}
