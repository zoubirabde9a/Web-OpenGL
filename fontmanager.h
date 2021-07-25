#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "SpriteFont.h"
#include "hash.h"

class FontManager {
    public:
public:
    static SpriteFont* get(const std::string &path, int size);
    static SpriteFont* get(const std::string& path, const char* buffer, size_t bufferSize, int size);
private:
    static std::unordered_map < Key, SpriteFont* > m_map;
};

#endif // FONTMANAGER_H
