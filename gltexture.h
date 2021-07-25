#ifndef GLTEXTURE
#define GLTEXTURE
#include <string>

struct GLTexture {
    std::string filePath;
    bool softFiltering;
    unsigned int id;
    int width;
    int height;
};


#endif // GLTEXTURE

