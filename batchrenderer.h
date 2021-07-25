#ifndef BATCHRENDERER_H
#define BATCHRENDERER_H


#include "glslprogram.h"
#include "spritebatch.h"
#include "debugrenderer.h"
#include "spritefont.h"


class Batch {
public:
    enum Type {
        SPRITE, SPRITES, QUAD, VERTICES, LIGHT, SHAPE, RECTANGLE, CIRCLE, FONT, FONT_BOUNDRY, COLOR_FONT, COLOR_FONT_BOUNDRY,
        CUBE
    };
public:
    // Sprite Constructor
    Batch(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, GLSLProgram* program, const ColorRGBA8& color, float angle);
    // Sprites Constructor
    Batch(const std::vector < Sprite >& sprites, GLSLProgram* program);
    // Quad Constructor
    Batch(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
        const Vertex& bottomRight, GLuint texture, float depth, GLSLProgram* program);
    // Vertices Constructor
    Batch(Vertex* verts, size_t vertsSize, GLuint texture, float depth, GLSLProgram* program);
    // Light Constructor
    Batch(const glm::vec2 &position, float radius, GLuint texture, float depth, GLSLProgram* program, const ColorRGBA8& color);
    // Shape Constructor
    Batch(const float* points, size_t nbPoints, float depth, const ColorRGBA8& color);
    // Rectangle Constructor
    Batch(const glm::vec4 &destRect, float depth, const ColorRGBA8& color, float angle);
    // Circle Constructor
    Batch(const glm::vec2 &position, float radius, float depth, const ColorRGBA8& color);
    // Font Constructor
    Batch(SpriteFont* font, const glm::vec2 &position, const std::string &text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just);
    // Font Boundry Constructor
    Batch(SpriteFont* font, const glm::vec2 &position, const std::string &text, const glm::vec2 &scalling, float depth, const glm::vec4& boundry, GLSLProgram* program, const ColorRGBA8& color, Justification just);
    // Color Font Constructor
    Batch(SpriteFont* font, const glm::vec2 &position, const TextRGB8 &text, const glm::vec2 &scalling, float depth, GLSLProgram* program, Justification just, GLubyte alpha);
    // Color Font Boundry Constructor
    Batch(SpriteFont* font, const glm::vec2 &position, const TextRGB8 &text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, Justification just, GLubyte alpha);

   /* glm::vec4 getSpriteDestRect() const;
    glm::vec2 getSpritePosition() const;
    glm::vec4 getSpriteUvs() const;
    ColorRGBA8 getSpriteColor() const;
    GLuint getSpriteTexture() const;
    float getSpriteAngle() const;
    glm::vec2 getScalling() const;*/
    GLSLProgram* getProgram();
   // float getRadius() const;



    Type type = SPRITE;
    float depth = 0.f;
    GLSLProgram* program = nullptr;
    union {
        // Sprite
        struct {
            GLuint s_texture;
            float s_x, s_y, s_z, s_w;
            float s_uvx, s_uvy, s_u, s_v;
            GLubyte s_r, s_g, s_b, s_a;
            float s_angle;
        };
        // SPRITES
        struct {
            GLuint ss_texture;
            Sprite* ss_sprites;
            size_t ss_size;
        };
        // Quad
        struct {
            GLuint q_texture;
            Vertex q_topLeft;
            Vertex q_bottomLeft;
            Vertex q_topRight;
            Vertex q_bottomRight;
        };
        // Vertices
        struct {
            GLuint v_texture;
            Vertex* v_verts;
            size_t v_vertsSize;
        };
        // Light
        struct {
            GLuint l_texture;
            float l_x, l_y;
            float l_radius;
            GLubyte l_r, l_g, l_b, l_a;
        };
        // Shape
        struct {
            GLuint sh_texture;
            const float* sh_points;
            size_t sh_nbPoints;
            GLubyte sh_r, sh_g, sh_b, sh_a;
        };
        // Rectangle
        struct {
            GLuint r_texture;
            float r_x, r_y, r_z, r_w;
            GLubyte r_r, r_g, r_b, r_a;
            float r_angle;
        };
        // Circle
        struct {
            GLuint c_texture;
            float c_x, c_y;
            float c_radius;
            GLubyte c_r, c_g, c_b, c_a;
        };
        // Font
        struct {
            SpriteFont* f_font;
            float f_x, f_y, f_sx, f_sy;
            char* f_text;
            GLubyte f_r, f_g, f_b, f_a;
            Justification f_just;
        };
        // Font Boundry
        struct {
            SpriteFont* fb_font;
            float fb_x, fb_y, fb_sx, fb_sy;
            char* fb_text;
            GLubyte fb_r, fb_g, fb_b, fb_a;
            Justification fb_just;
            float fb_bx, fb_by, fb_bz, fb_bw;
        };
        // Color Font
        struct {
            SpriteFont* cf_font;
            float cf_x, cf_y, cf_sx, cf_sy;
            TextRGB8* cf_colorText;
            GLubyte cf_alpha;
            Justification cf_just;
        };
        // color Font Boundry
        struct {
            SpriteFont* cfb_font;
            float cfb_x, cfb_y, cfb_sx, cfb_sy;
            TextRGB8* cfb_colorText;
            GLubyte cfb_alpha;
            Justification cfb_just;
            float cfb_bx, cfb_by, cfb_bz, cfb_bw;
        };
    };
};
enum BatchSortType {
    NONE, BACK_TO_FRONT, FRONT_TO_BACK, PROGRAM
};
class BatchRenderer {
public:
    void init();
    void dispose();
    void begin(BatchSortType sortType = BatchSortType::NONE);
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, GLSLProgram* prog, const ColorRGBA8& color, float angle = 0.0f);
    void draw(Vertex* verts, size_t vertsSize, GLuint texture, float depth, GLSLProgram* program);
    void draw(const Vertex& topLeft, const Vertex& bottomLeft, const Vertex& topRight,
        const Vertex& bottomRight, GLuint texture, float depth, GLSLProgram* program);
    void drawSprites(const std::vector < Sprite > &sprites, GLSLProgram* program);
    void drawLight(const glm::vec2 &position, float radius, GLuint texture, float depth, GLSLProgram* prog, const ColorRGBA8 &color);
    void drawShape(const float* points, size_t nbPoints, float depth, const ColorRGBA8& color);
    void drawRectangle(const glm::vec4& rect, float depth, const ColorRGBA8& color, float angle = 0.0f);
    void drawCircle(const glm::vec2 &position, float radius, float depth, const ColorRGBA8& color);
    void drawText(SpriteFont* font, const glm::vec2 &position, const char* text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just);
    void drawText(SpriteFont* font, const glm::vec2 &position, const std::string& text, const glm::vec2 &scalling, float depth, GLSLProgram* program, const ColorRGBA8& color, Justification just);
    void drawText(SpriteFont* font, const glm::vec2 &position, const std::string& text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, const ColorRGBA8& color, Justification just);
    void drawText(SpriteFont* font, const glm::vec2 &position, const TextRGB8& text, const glm::vec2 &scalling, float depth, GLSLProgram* program, Justification just, GLubyte alpha);
    void drawText(SpriteFont* font, const glm::vec2 &position, const TextRGB8& text, const glm::vec2 &scalling, float depth, const glm::vec4 &boundry, GLSLProgram* program, Justification just, GLubyte alpha);
    void render(const glm::mat4 &projectionMat);
private:
    void createBatchPtrs();
    void sortBatchPtrs();
private:
    BatchSortType m_sortType;
    SpriteBatch m_spriteBatch;
    DebugRenderer m_debugRenderer;
    std::vector < Batch > m_batches;
    std::vector < Batch* > m_batchPtrs;
};
#endif // BATCHRENDERER_H
