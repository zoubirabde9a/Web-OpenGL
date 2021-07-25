#include "imageloader.h"
#include "picopng.h"
#include "iomanager.h"
#include "opengl.h"

GLTexture ImageLoader::loadImage(const char* filePath, unsigned long width,  unsigned long height, const unsigned char* buffer, bool softFiltering){
    GLTexture texture = {};

    //Generate the openGL texture object
    glGenTextures(1, &(texture.id));

    //Bind the texture object
    glBindTexture(GL_TEXTURE_2D, texture.id);
    //Upload the pixels to the texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    //Set some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    setTextureSoftFilter(softFiltering);

    //Generate the mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = width;
    texture.height = height;
    texture.filePath = filePath;
    texture.softFiltering = softFiltering;
    //Return a copy of the texture data
    return texture;
}

GLTexture ImageLoader::loadPng(const char* filePath, bool softFiltering){
    Buffer buffer = IOManager::readFile(filePath);
    return loadPng(filePath, (const unsigned char*)buffer.data, buffer.size, softFiltering);
}

GLTexture ImageLoader::loadPng(const char* filePath, const unsigned char* buffer, unsigned long bufferSize, bool softFiltering){
    std::vector<unsigned char> out;
    unsigned long width, height;
    GLTexture texture = {};

    //Decode the .png format into an array of pixels
    int errorCode = decodePNG(out, width, height, buffer, bufferSize);
    if (errorCode != 0) {
        fprintf(stderr, "%s decode png failed with error %d", filePath, errorCode);
        texture.width = 0;
        texture.height = 0;
        texture.filePath = "Invalid Path";
        return texture;
    }

    return loadImage(filePath, width, height, &out[0], softFiltering);
}
void ImageLoader::setTextureSoftFilter(bool softFilter){
    if (softFilter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
}

