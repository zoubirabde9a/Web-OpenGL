#ifndef TILESHEET_H
#define TILESHEET_H


#include "gltexture.h"
#include <glm\glm.hpp>
#include <vector>

class TileSheet {
public:
    static glm::vec4 getUvs(glm::ivec4 Rect, const GLTexture* texture);
public:
    TileSheet(){}
    TileSheet(const char* filePath, const glm::ivec2 &tileDims, bool softFiletring = true);
    TileSheet(const char* filePath, bool softFiletring = true);
    void init(const char* filePath, const glm::ivec2 &tileDims, bool softFiletring = true);
    void init(const char* filePath, bool softFiletring = true);
    void setDims(const glm::ivec2 &tileDims);
    int getMaxIndex() const;
    glm::vec4 getUvs(int index) const;
    int getIndex(const glm::vec4 &uvs) const;
    glm::vec4 getUvs(glm::ivec4 Rect) const;

public:
    inline void setTexture(const GLTexture* texture) { m_texture = texture; }
    inline unsigned int getTextureId() const { return m_texture->id; }
    inline const GLTexture* getTexture() const { return m_texture; }
    inline const std::string &getFilePath() const { return m_texture->filePath; }
    inline const glm::ivec2 &getTileDims() const { return m_tileDims; }

private:
    const GLTexture* m_texture;
    glm::ivec2 m_tileDims;

};



#endif // TILESHEET_H
