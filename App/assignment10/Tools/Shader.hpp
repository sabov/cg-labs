////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////

#include <gl_core_32.hh>
#include <string>

class Shader {
public:
    Shader(const std::string &fileName, GLenum type) :
            mObjectName(0), mType(type) {
        mObjectName = glCreateShader(mType);
        setFromFile(fileName);
    }

    ~Shader() {
        glDeleteShader(mObjectName);
    }

    inline GLuint getObjectName() const {
        return mObjectName;
    }

    bool setFromFile(const std::string &fileName) {
        return setSource(getFileContent(fileName));
    }

    bool setSource(const std::string &source) {

        const char *source_c = source.c_str();
        glShaderSource(mObjectName, 1, &source_c, NULL);
        glCompileShader(mObjectName);

        // check for shader compile errors:
        GLint shaderError;
        glGetShaderiv(mObjectName, GL_COMPILE_STATUS, &shaderError);
        if(shaderError != GL_TRUE) {
            // yes, errors
            std::cerr << "Shader compile error: " << std::endl;
        }

        // a log gets always printed (could be warnings)
        GLsizei length = 0;
        glGetShaderiv(mObjectName, GL_INFO_LOG_LENGTH, &length);
        if(length > 1) {
            // a compile log can get produced even if there were no errors, but warnings!
            GLchar* pInfo = new char[length + 1];
            glGetShaderInfoLog(mObjectName, length, &length, pInfo);
            std::cerr << "Compile log: " << std::string(pInfo) << std::endl;
            delete[] pInfo;
        }
        return (shaderError == GL_TRUE);
    }

private:

    std::string getFileContent(const std::string &fileName) {

        std::string line = "";
        std::string fileContent = "";

        std::ifstream fileStream(fileName.c_str(), std::ifstream::in);

        if(fileStream.is_open()) {
            while(fileStream.good()) {
                std::getline(fileStream, line);
                fileContent += line + "\n";
            }
            fileStream.close();
        } else {
            std::cout << "Failed to open file: " << fileName << std::endl;
            return "";
        }
        return fileContent;
    }

    GLuint mObjectName;
    GLenum mType;
};
