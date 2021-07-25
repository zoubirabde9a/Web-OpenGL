#include "spritefont.h"
#include "imageloader.h"

using namespace ftgl;
static const char* ALL_CHARACTERS = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
SpriteFont::SpriteFont(const char* filePath, size_t fontSize)
: m_path(filePath), m_size(fontSize){
    m_FTAtlas = texture_atlas_new(1024, 1024);
    m_FTFont = texture_font_new_from_file(m_FTAtlas, fontSize, filePath);
    texture_font_load_glyphs(m_FTFont, ALL_CHARACTERS);
    glGenTextures(1, &(m_FTAtlas->id));
    glBindTexture(GL_TEXTURE_2D, m_FTAtlas->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, 1024, 1024, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_FTAtlas->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);



}
SpriteFont::SpriteFont(const char* buffer, size_t size, size_t fontSize) {
    m_FTAtlas = texture_atlas_new(1024, 1024);
    m_FTFont = texture_font_new_from_memory(m_FTAtlas, fontSize, buffer, size);
    texture_font_load_glyphs(m_FTFont, ALL_CHARACTERS);
    glGenTextures(1, &(m_FTAtlas->id));
    glGenTextures(1, &(m_FTAtlas->id));
    glBindTexture(GL_TEXTURE_2D, m_FTAtlas->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, 1024, 1024, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_FTAtlas->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteFont::init(int width, int height){

}

SpriteFont::~SpriteFont()
{
    texture_font_delete(m_FTFont);
    texture_atlas_delete(m_FTAtlas);
}

glm::vec2 SpriteFont::measure(const char* text) {
    glm::vec2 size(0, getFontHeight());
    float cw = 0;
    for (int si = 0; text[si] != 0; si++) {
        char c = text[si];
        if (c == '\n') {
            size.y += getFontHeight();
            size.x = size.x * (size.x >= cw) + cw * (size.x < cw);
            cw = 0;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    size.x = size.x * (size.x >= cw) + cw * (size.x < cw);
    return size;
}
glm::vec2 SpriteFont::measure(const std::string& text) {
    return measure(text.c_str());
}
glm::vec2 SpriteFont::measure(const TextRGB8& text) {
    glm::vec2 size(0, getFontHeight());
    float cw = 0;
    for (int si = 0; si < text.size(); si++) {
        char c = text.toCString()[si];
        if (c == '\n') {
            size.y += getFontHeight();
            size.x = size.x * (size.x >= cw) + cw * (size.x < cw);
            cw = 0;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text.toCString()[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    size.x = size.x * (size.x >= cw) + cw * (size.x < cw);
    return size;
}
float SpriteFont::mesureLine(const char* text) {
    float cw = 0;
    for (int si = 0; text[si] != 0; si++) {
        char c = text[si];
        if (c == '\n') {
            return cw;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    return cw;
}
float SpriteFont::mesureLine(const std::string& text) {
    return mesureLine(text.c_str());
}
float SpriteFont::mesureLine(const TextRGB8& text) {
    float cw = 0;
    for (int si = 0; si < text.size(); si++) {
        char c = text.toCString()[si];
        if (c == '\n') {
            return cw;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text.toCString()[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    return cw;
}
float SpriteFont::mesureLine(const char* text, size_t offset, size_t nb) {
    float cw = 0;
    for (int si = offset; text[si] != 0 && si < nb + 1; si++) {
        char c = text[si];
        if (c == '\n') {
            return cw;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    return cw;
}
float SpriteFont::mesureLine(const std::string& text, size_t offset, size_t nb) {
    float cw = 0;
    for (int si = offset; si < text.size() && si < nb; si++) {
        char c = text[si];
        if (c == '\n') {
            return cw;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    return cw;
}
float SpriteFont::mesureLine(const TextRGB8& text, size_t offset, size_t nb) {
    float cw = 0;
    for (int si = offset; si < text.size() && si < nb; si++) {
        char c = text.toCString()[si];
        if (c == '\n') {
            return cw;
        }
        else {
            texture_glyph_t* glyph = getGlyph(c);
            if (glyph != NULL) {
                if (si > 0) {
                    float kerning = texture_glyph_get_kerning(glyph, &text.toCString()[si - 1]);
                    cw += kerning;
                }
                cw += glyph->advance_x;
            }
        }
    }
    return cw;
}
void SpriteFont::draw(SpriteBatch& batch, const char* text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, ColorRGBA8 tint,
    Justification just) {
    glm::vec2 tp = position;
    glm::vec4 destRect;
    glm::vec4 uvs;

    // Apply justification
    if (just == Justification::MIDDLE) {
        tp.x -= mesureLine(text) * scalling.x / 2.0f;
    }
    else if (just == Justification::RIGHT) {
        tp.x -= mesureLine(text) * scalling.x;
    }
    for (int si = 0; text[si] != 0; si++) {
        char c = text[si];
        texture_glyph_t* glyph = getGlyph(c);
        if (glyph != NULL) {
            if (tp.x > position.x) {
                float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                tp.x += kerning * scalling.x;
            }
            if (c == '\n') {
                tp.y -= getFontHeight() * scalling.y;
                tp.x = position.x;
            }
            else {
                destRect.x = tp.x + glyph->offset_x * scalling.x;
                destRect.y = tp.y - ((int)glyph->height - glyph->offset_y + m_FTFont->descender) * scalling.y;
                destRect.z = glyph->width * scalling.x;
                destRect.w = glyph->height * scalling.y;
                uvs.x = glyph->s0;
                uvs.y = 1.0f - glyph->t1;
                uvs.z = glyph->s1 - glyph->s0;
                uvs.w = glyph->t1 - glyph->t0;
                batch.draw(destRect, uvs, m_FTAtlas->id, depth, tint);
                tp.x += glyph->advance_x * scalling.x;
            }
        }
    }
}
void SpriteFont::draw(SpriteBatch& batch, const char* text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, const glm::vec4 &boundaries, ColorRGBA8 tint,
    Justification just) {
    glm::vec2 tp = position;
    glm::vec4 destRect;
    glm::vec4 uvs;
    // Apply justification
    if (just == Justification::MIDDLE) {
        tp.x -= mesureLine(text) * scalling.x / 2.0f;
    }
    else if (just == Justification::RIGHT) {
        tp.x -= mesureLine(text) * scalling.x;
    }
    for (int si = 0; text[si] != 0; si++) {
        char c = text[si];
        texture_glyph_t* glyph = getGlyph(c);
        if (glyph != NULL) {
            if (tp.x > position.x) {
                float kerning = texture_glyph_get_kerning(glyph, &text[si - 1]);
                tp.x += kerning * scalling.x;
            }
            if (c == '\n') {
                tp.y -= getFontHeight() * scalling.y;
                tp.x = position.x;
            }
            else {
                destRect.x = tp.x + glyph->offset_x * scalling.x;
                destRect.y = tp.y - ((int)glyph->height - glyph->offset_y + m_FTFont->descender) * scalling.y;
                destRect.z = glyph->width * scalling.x;
                destRect.w = glyph->height * scalling.y;
                uvs.x = glyph->s0;
                uvs.y = 1.0f - glyph->t1;
                uvs.z = glyph->s1 - glyph->s0;
                uvs.w = glyph->t1 - glyph->t0;
                applyBoundry(uvs, destRect, boundaries);
                batch.draw(destRect, uvs, m_FTAtlas->id, depth, tint);
                tp.x += glyph->advance_x * scalling.x;
            }
        }
    }
}
void SpriteFont::draw(SpriteBatch& batch, const std::string& text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, ColorRGBA8 tint,
    Justification just) {
    draw(batch, text.c_str(), position, scalling, depth, tint, just);
}

void SpriteFont::draw(SpriteBatch& batch, const std::string& text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, const glm::vec4 &boundaries, ColorRGBA8 tint,
    Justification just) {
    draw(batch, text.c_str(), position, scalling, depth, boundaries, tint, just);
}
void SpriteFont::draw(SpriteBatch& batch, const TextRGB8& text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, Justification just, GLubyte alpha) {
    glm::vec2 tp = position;
    glm::vec4 destRect;
    glm::vec4 uvs;

    // Apply justification
    if (just == Justification::MIDDLE) {
        tp.x -= mesureLine(text) * scalling.x / 2.0f;
    }
    else if (just == Justification::RIGHT) {
        tp.x -= mesureLine(text) * scalling.x;
    }
    for (int si = 0; si < text.size(); si++) {
        char c = text.toCString()[si];
        const ColorRGB8& color = text.getColors()[si];
        texture_glyph_t* glyph = getGlyph(c);
        if (glyph != NULL) {
            if (tp.x > position.x) {
                float kerning = texture_glyph_get_kerning(glyph, &text.toCString()[si - 1]);
                tp.x += kerning * scalling.x;
            }
            if (c == '\n') {
                tp.y -= getFontHeight() * scalling.y;
                tp.x = position.x;
            }
            else {
                destRect.x = tp.x + glyph->offset_x * scalling.x;
                destRect.y = tp.y - ((int)glyph->height - glyph->offset_y + m_FTFont->descender) * scalling.y;
                destRect.z = glyph->width * scalling.x;
                destRect.w = glyph->height * scalling.y;
                uvs.x = glyph->s0;
                uvs.y = 1.0f - glyph->t1;
                uvs.z = glyph->s1 - glyph->s0;
                uvs.w = glyph->t1 - glyph->t0;
                batch.draw(destRect, uvs, m_FTAtlas->id, depth, ColorRGBA8(color, alpha));
                tp.x += glyph->advance_x * scalling.x;
            }
        }
    }
}

void SpriteFont::draw(SpriteBatch &batch, const TextRGB8& text, const glm::vec2 &position,
    const glm::vec2 &scalling, float depth, const glm::vec4 &boundaries,
    Justification just, GLubyte alpha) {
    glm::vec2 tp = position;
    glm::vec4 destRect;
    glm::vec4 uvs;

    // Apply justification
    if (just == Justification::MIDDLE) {
        tp.x -= mesureLine(text) * scalling.x / 2.0f;
    }
    else if (just == Justification::RIGHT) {
        tp.x -= mesureLine(text) * scalling.x;
    }
    for (int si = 0; si < text.size(); si++) {
        char c = text.toCString()[si];
        const ColorRGB8& color = text.getColors()[si];
        texture_glyph_t* glyph = getGlyph(c);
        if (glyph != NULL) {
            if (tp.x > position.x) {
                float kerning = texture_glyph_get_kerning(glyph, &text.toCString()[si - 1]);
                tp.x += kerning * scalling.x;
            }
            if (c == '\n') {
                tp.y -= getFontHeight() * scalling.y;
                tp.x = position.x;
            }
            else {
                destRect.x = tp.x + glyph->offset_x * scalling.x;
                destRect.y = tp.y - ((int)glyph->height - glyph->offset_y + m_FTFont->descender) * scalling.y;
                destRect.z = glyph->width * scalling.x;
                destRect.w = glyph->height * scalling.y;
                uvs.x = glyph->s0;
                uvs.y = 1.0f - glyph->t1;
                uvs.z = glyph->s1 - glyph->s0;
                uvs.w = glyph->t1 - glyph->t0;
                applyBoundry(uvs, destRect, boundaries);
                batch.draw(destRect, uvs, m_FTAtlas->id, depth, ColorRGBA8(color, alpha));
                tp.x += glyph->advance_x * scalling.x;
            }
        }
    }
}
int SpriteFont::getFontHeight() const {
    return m_FTFont->height;
}
texture_glyph_t* SpriteFont::getGlyph(char c) const {
    return texture_font_get_glyph(m_FTFont, &c);
}
//#endif


