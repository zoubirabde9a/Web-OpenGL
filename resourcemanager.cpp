#include "resourcemanager.h"
#include "imageloader.h"
#include "opengl.h"

const GLTexture* ResourceManager::getTexture(const char* texturePath, bool softFiltering){
    auto it = m_textureMap.find(texturePath);
    if (it != m_textureMap.end()){
        if (it->second.softFiltering != softFiltering){
            it->second.softFiltering = softFiltering;
            glBindTexture(GL_TEXTURE_2D, it->second.id);
            ImageLoader::setTextureSoftFilter(softFiltering);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        return &it->second;
    } else {
        return &m_textureMap.insert(std::pair < std::string, GLTexture >(texturePath, ImageLoader::loadPng(texturePath, softFiltering))).first->second;
    }

}

