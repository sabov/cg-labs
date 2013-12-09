////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_DATA
#define TEXTURE_DATA

#include <gl_core_32.hh>

class TextureData {
    // ========================================================================================================= \/
    // ============================================================================================ CONSTRUCTORS \/
    // ========================================================================================================= \/
public:
    TextureData(void) :
            pData(NULL), width(0), height(0), depth(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {
    }
    virtual ~TextureData(void) {
        delete[] pData;
    }

    // ========================================================================================================= \/
    // ================================================================================================= GETTERS \/
    // ========================================================================================================= \/
public:
    // the pixel data
    GLubyte* getData(void) const {
        return pData;
    }
    // width in pixels
    GLsizei getWidth(void) const {
        return width;
    }
    // height in pixels
    GLsizei getHeight(void) const {
        return height;
    }
    // depth in pixels in case of a 3D texture
    GLsizei getDepth(void) const {
        return depth;
    }
    // the opengl format enum for the data
    GLenum getFormat(void) const {
        return format;
    }
    // the opengl type enum for the data
    GLenum getType(void) const {
        return type;
    }

    // ========================================================================================================= \/
    // ================================================================================================= SETTERS \/
    // ========================================================================================================= \/
public:
    void setData(GLubyte* _pData) {
        pData = _pData;
    }
    void setWidth(GLsizei _width) {
        width = _width;
    }
    void setHeight(GLsizei _height) {
        height = _height;
    }
    void setDepth(GLsizei _depth) {
        depth = _depth;
    }
    void setFormat(GLenum _format) {
        format = _format;
    }
    void setType(GLenum _type) {
        type = _type;
    }

    // ========================================================================================================= \/
    // ================================================================================================== FIELDS \/
    // ========================================================================================================= \/
private:
    GLubyte* pData;
    GLsizei width;
    GLsizei height;
    GLsizei depth;
    GLenum format;
    GLenum type;
};

#endif // TEXTUREDATA_HH
