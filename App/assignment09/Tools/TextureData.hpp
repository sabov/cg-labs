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
    GLubyte* getData(void) const {
        return pData;
    }
    GLsizei getWidth(void) const {
        return width;
    }
    GLsizei getHeight(void) const {
        return height;
    }
    GLsizei getDepth(void) const {
        return depth;
    }
    GLenum getFormat(void) const {
        return format;
    }
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
