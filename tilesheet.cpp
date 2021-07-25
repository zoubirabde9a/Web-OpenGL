#include "tilesheet.h"
#include "resourcemanager.h"

glm::vec4 TileSheet::TileSheet::getUvs(glm::ivec4 Rect, const GLTexture* texture) {
    glm::vec4 uv;
    uv.x = (float)Rect.x / (float)texture->width;
    uv.y = (float)Rect.y / (float)texture->height;
    uv.z = (float)Rect.z / (float)texture->width;
    uv.w = (float)Rect.w / (float)texture->height;
    return uv;
}
TileSheet::TileSheet(const char* filePath, const glm::ivec2 &tileDims, bool softfiltering) {
    init(filePath, tileDims, softfiltering);
}
TileSheet::TileSheet(const char* filePath, bool softFiletring){
    init(filePath, softFiletring);
}

void TileSheet::init(const char* filePath, bool softFiletring) {
    m_texture = ResourceManager::getSingelton().getTexture(filePath, softFiletring);
    m_tileDims.x = m_texture->width;
    m_tileDims.y = m_texture->height;
}
void TileSheet::init(const char* filePath, const glm::ivec2 &tileDims, bool softFiltering) {
    m_texture = ResourceManager::getSingelton().getTexture(filePath, softFiltering);
    m_tileDims = tileDims;
    m_tileDims.x = m_tileDims.x * (m_tileDims.x <= m_texture->width) + m_texture->width * !(m_tileDims.x <= m_texture->width);
    m_tileDims.y = m_tileDims.y * (m_tileDims.y <= m_texture->height) + m_texture->height * !(m_tileDims.y <= m_texture->height);
}
void TileSheet::setDims(const glm::ivec2 &tileDims) {
    m_tileDims = tileDims;
}
int TileSheet::getMaxIndex() const {
    const glm::ivec2 TilesNumber(m_texture->width / m_tileDims.x, m_texture->height / m_tileDims.y);
    return TilesNumber.x * TilesNumber.y - 1;
}
glm::vec4 TileSheet::getUvs(int index) const {
    const glm::ivec2 TilesNumber(m_texture->width / m_tileDims.x, m_texture->height / m_tileDims.y);
    int xTile = index % TilesNumber.x;
    int yTile = floor((float)index / (float)TilesNumber.x);

    glm::vec4 uvs;
    uvs.x = (float)xTile / (float)TilesNumber.x;
    uvs.y = (float)yTile / (float)TilesNumber.y;
    uvs.z = 1.0f / (float)TilesNumber.x;
    uvs.w = 1.0f / (float)TilesNumber.y;
    return uvs;
}
int TileSheet::getIndex(const glm::vec4 &uvs) const {
    return uvs.x * (float)m_texture->width / m_tileDims.x ;
}
glm::vec4 TileSheet::getUvs(glm::ivec4 Rect) const {
    glm::vec4 uv;
    uv.x = (float)Rect.x / (float)m_texture->width;
    uv.y = (float)Rect.y / (float)m_texture->height;
    uv.z = (float)Rect.z / (float)m_texture->width;
    uv.w = (float)Rect.w / (float)m_texture->height;
    return uv;
}
