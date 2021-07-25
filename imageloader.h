#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include "gltexture.h"

class ImageLoader
{
public:
    static GLTexture loadImage(const char* filePath, unsigned long width,  unsigned long height, const unsigned char* buffer, bool softFiltering = true);
    static GLTexture loadPng(const char* filePath, bool softFiltering = true);
    static GLTexture loadPng(const char* filePath, const unsigned char* buffer, unsigned long bufferSize, bool softFiltering = true);
    static void setTextureSoftFilter(bool softFilter);
};

#endif // IMAGELOADER_H
