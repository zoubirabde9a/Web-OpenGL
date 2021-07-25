#ifndef SPRITEFONT_H
#define SPRITEFONT_H

#include "SpriteBatch.h"
#include <string>
#include "freetypegl.h"

class SpriteFont
{
public:
    SpriteFont(const char* filePath, size_t fontSize);
    SpriteFont(const char* buffer, size_t size, size_t fontSize);
    ~SpriteFont();
    glm::vec2 measure(const char* text);
    glm::vec2 measure(const std::string& text);
    glm::vec2 measure(const TextRGB8& text);
    float mesureLine(const char* text);
    float mesureLine(const std::string& text);
    float mesureLine(const TextRGB8& text);
    float mesureLine(const char* text, size_t offset, size_t nb);
    float mesureLine(const std::string& text, size_t offset, size_t nb);
    float mesureLine(const TextRGB8& text, size_t offset, size_t nb);

    /// Draws using a spritebatch
    void draw(SpriteBatch& batch, const char* text, const glm::vec2 &position,
        const glm::vec2 &scaling, float depth, ColorRGBA8 tint,
        Justification just = Justification::LEFT);

    void draw(SpriteBatch& batch, const char* text, const glm::vec2 &position,
        const glm::vec2 &scaling, float depth, const glm::vec4 &boundaries, ColorRGBA8 tint,
        Justification just = Justification::LEFT);

    void draw(SpriteBatch& batch, const std::string& text, const glm::vec2 &position,
        const glm::vec2 &scaling, float depth, ColorRGBA8 tint,
        Justification just = Justification::LEFT);

    void draw(SpriteBatch& batch, const std::string& text, const glm::vec2 &position,
        const glm::vec2 &scaling, float depth, const glm::vec4 &boundaries, ColorRGBA8 tint,
        Justification just = Justification::LEFT);


    void draw(SpriteBatch& batch, const TextRGB8& text, const glm::vec2 &position,
        const glm::vec2 &scalling, float depth, Justification just = Justification::LEFT
        , GLubyte alpha = 255);

    void draw(SpriteBatch &batch, const TextRGB8& text, const glm::vec2 &position,
        const glm::vec2 &scalling, float depth, const glm::vec4 &boundaries,
        Justification just = Justification::LEFT, GLubyte alpha = 255);

    int getFontHeight() const;
    ftgl::texture_glyph_t* getGlyph(char c) const;
    inline const std::string getPath() const { return m_path; }
    inline size_t getSize() const { return m_size; }
private:
    void init(int width, int height);
private:
    std::string m_path;
    size_t m_size;
    ftgl::texture_atlas_t* m_FTAtlas = nullptr;
    ftgl::texture_font_t* m_FTFont = nullptr;
};

#endif // SPRITEFONT_H
