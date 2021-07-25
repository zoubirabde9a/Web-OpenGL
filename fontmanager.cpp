#include "fontmanager.h"

std::unordered_map < Key, SpriteFont* > FontManager::m_map;
SpriteFont* FontManager::get(const std::string &path, int size) {
    auto it = m_map.find({path, size});
    if (it == m_map.end()) {
        SpriteFont* spriteFont = new SpriteFont(path.c_str(), size);
        Key key(path, size);
        std::pair < Key, SpriteFont* > pair = { key, spriteFont };
        return (m_map.insert(pair).first->second);
    }
    else return it->second;
}
SpriteFont* FontManager::get(const std::string& path, const char* buffer, size_t bufferSize, int size) {
    auto it = m_map.find({ path, size });
    if (it == m_map.end()) {
        SpriteFont* spriteFont = new SpriteFont(buffer, bufferSize, size);
        Key key(path, size);
        std::pair < Key, SpriteFont* > pair = { key, spriteFont };
        return (m_map.insert(pair).first->second);
    }
    else return it->second;
}


