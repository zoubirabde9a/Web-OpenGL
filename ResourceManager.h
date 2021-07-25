#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "imageloader.h"
#include <unordered_map>
#include "tilesheet.h"

class ResourceManager
{
public:
    static ResourceManager& getSingelton(){
        static ResourceManager singelton;
        return singelton;
    }
    const GLTexture* getTexture(const char* texturePath, bool softFiltering = true);
private:
    std::unordered_map < std::string, GLTexture > m_textureMap;
};

#endif // RESOURCEMANAGER_H
