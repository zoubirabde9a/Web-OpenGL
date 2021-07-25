#include "glslprogram.h"
#include "IOManager.h"

#include <vector>

#include <fstream>
    GLSLProgram::~GLSLProgram()
    {
        destroy();
    }

    //Compiles the shaders into a form that your GPU can understand
    void GLSLProgram::compileShaders(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
        const char* vertSource = IOManager::readFileToString(vertexShaderFilePath);
        const char* fragSource = IOManager::readFileToString(fragmentShaderFilePath);
        compileShadersFromSource(vertSource, fragSource);
    }
    void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) {
        m_numAttributes = 0;
        //Vertex and fragment shaders are successfully compiled.
        //Now time to link them together into a program.
        //Get a program object.
        m_programID = glCreateProgram();

        //Create the vertex shader object, and store its ID
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (m_vertexShaderID == 0) {
            IOManager::fatalError("Vertex shader failed to be created!");
        }

        //Create the fragment shader object, and store its ID
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (m_fragmentShaderID == 0) {
            IOManager::fatalError("Fragment shader failed to be created!");
        }

        //Compile each shader
        compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
        compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);
    }
    void GLSLProgram::linkShaders() {

        //Attach our shaders to our program
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);

        //Link our program
        glLinkProgram(m_programID);

        //Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);



            //We don't need the program anymore.
            glDeleteProgram(m_programID);
            //Don't leak shaders either.
            glDeleteShader(m_vertexShaderID);
            glDeleteShader(m_fragmentShaderID);

            //print the error log and quit
            std::printf("%s\n", &(errorLog[0]));
            IOManager::fatalError("Shaders failed to link!");
        }

        //Always detach shaders after a successful link.
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
    }

    //Adds an attribute to our shader. SHould be called between compiling and linking.
    void GLSLProgram::addAttribute(const char* attributeName) {
        glBindAttribLocation(m_programID, m_numAttributes++, attributeName);
    }

    GLint GLSLProgram::getUniformLocation(const char* uniformName) {
    GLint location = glGetUniformLocation(m_programID, uniformName);
#ifdef EMSCRIPTEN
       if (location == GL_INVALID_VALUE) {
#else
       if (location == GL_INVALID_INDEX) {
#endif
            fprintf(stderr, "Uniform %s not found in Shader !", uniformName);
            IOManager::fatalError("");
        }
        return location;
    }
    void GLSLProgram::setUniformf(const char* name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }
    void GLSLProgram::setUniformf(const char* name, float x, float y, float z) {
        glUniform3f(getUniformLocation(name), x, y, z);
    }
    void GLSLProgram::setUniformf(const char* name, float x, float y) {
        glUniform2f(getUniformLocation(name), x, y);
    }
    void GLSLProgram::setUniformf(const char* name, float x) {
        glUniform1f(getUniformLocation(name), x);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, float x, float y, float z, float w) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]");
        glUniform4f(getUniformLocation(locationName.c_str()), x, y, z, w);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, float x, float y, float z) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]");
        glUniform3f(getUniformLocation(locationName.c_str()), x, y, z);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, float x, float y) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]");
        glUniform2f(getUniformLocation(locationName.c_str()), x, y);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, float x) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]");
        glUniform1f(getUniformLocation(locationName.c_str()), x);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y, float z, float w) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]." + std::string(attributeName));
        glUniform4f(getUniformLocation(locationName.c_str()), x, y, z, w);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y, float z) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]." + std::string(attributeName));
        glUniform3f(getUniformLocation(locationName.c_str()), x, y, z);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, const char* attributeName, float x, float y) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]." + std::string(attributeName));
        glUniform2f(getUniformLocation(locationName.c_str()), x, y);
    }
    void GLSLProgram::setUniformArrayf(const char* name, size_t position, const char* attributeName, float x) {
        std::string locationName(std::string(name) + "[" + std::to_string(position) + "]." + std::string(attributeName));
        glUniform1f(getUniformLocation(locationName.c_str()), x);
    }
    void GLSLProgram::setUniformi(const char* name, GLint x) {
        glUniform1i(getUniformLocation(name), x);
    }
    void GLSLProgram::setUniformui(const char* name, GLuint x) {
#ifdef EMSCRIPTEN
        glUniform1i(getUniformLocation(name), x);
#else
        glUniform1ui(getUniformLocation(name), x);
#endif
    }
    //enable the shader, and all its attributes
    void GLSLProgram::use() {
        glUseProgram(m_programID);
        //enable all the attributes we added with addAttribute
        for (int i = 0; i < m_numAttributes; i++) {
            glEnableVertexAttribArray(i);
        }
    }

    //disable the shader
    void GLSLProgram::unuse() {
        glUseProgram(0);
        for (int i = 0; i < m_numAttributes; i++) {
            glDisableVertexAttribArray(i);
        }
    }

    //Compiles a single shader file
    void GLSLProgram::compileShader(const char* source, const char* name, GLuint id) {

        //tell opengl that we want to use fileContents as the contents of the shader file
        glShaderSource(id, 1, &source, nullptr);

        //compile the shader
        glCompileShader(id);

        //check for errors
        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

            //Provide the infolog in whatever manor you deem best.
            //Exit with failure.
            glDeleteShader(id); //Don't leak the shader.

            //Print error log and quit
            std::printf("%s\n", &(errorLog[0]));
            fprintf(stderr, "Shader %s failed to compile !\n", name);
            getchar();
            exit(-1);
        }
    }


    void GLSLProgram::destroy() {
        if (m_programID)
            glDeleteProgram(m_programID);
    }

