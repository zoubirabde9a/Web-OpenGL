#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#include <string>

#include "opengl.h"

class GLSLProgram
{
public:
    ~GLSLProgram();
    void compileShaders(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    void compileShadersFromSource(const char* vertexSource,const char* fragmentSource);
    void linkShaders();

    void addAttribute(const char* attributeName);

    GLint getUniformLocation(const char* uniformName);

    void setUniformf(const char* name, float x, float y, float z, float w);
    void setUniformf(const char* name, float x, float y, float z);
    void setUniformf(const char* name, float x, float y);
    void setUniformf(const char* name, float x);
    void setUniformArrayf(const char* name, size_t position, float x, float y, float z, float w);
    void setUniformArrayf(const char* name, size_t position, float x, float y, float z);
    void setUniformArrayf(const char* name, size_t position, float x, float y);
    void setUniformArrayf(const char* name, size_t position, float x);
    void setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y, float z, float w);
    void setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y, float z);
    void setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y);
    void setUniformArrayf(const char* name, size_t position, const char* attributeName, float x);

    void setUniformi(const char* name, GLint x);
    void setUniformui(const char* name, GLuint x);

    void use();
    void unuse();
    void destroy();
private:
    void compileShader(const char* source,const char* name, GLuint id);
private:
    int m_numAttributes = 0;
    GLuint m_programID = 0;
    GLuint m_vertexShaderID = 0;
    GLuint m_fragmentShaderID = 0;
};


#endif // GLSLPROGRAM_H
