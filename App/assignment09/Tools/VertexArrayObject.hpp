////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////

#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

/**
 * A VertexArrayObject is a predefined combination of (multiple) attributes of
 * (multiple) ArrayBuffers and optionally one ElementArrayBuffer.
 *
 * It's only present in OpenGL 3.0 onwards. For older implementations (or
 * embedded systems) see VertexBufferObject which is a softwareimplementation
 * of the same idea).
 * Alternatively, there are the GL_APPLE_vertex_array_object and
 * GL_ARB_vertex_array_object extensions for OpenGL 2.1.
 * OES_vertex_array_object for OpenGL ES (e.g. iOS 4.0+)
 *
 * A VAO will cache the enabled vertex attributes (set with glEnableVertexAttribArray)
 * and vertex attribute pointer calls (glVertexAttribPointer).
 * Binding a VAO will restore that state (saving a lot of gl calls to do that
 * manually).
 */

#include <gl_core_32.hh>

#include "ArrayBuffer.hpp"
//#include "ElementArrayBuffer.h"
#include "ShaderProgram.hpp"

#include <vector>
#include <stdint.h>

class VertexArrayObject {
    // ==================================================================================================== \/
    // ============================================================================================ STRUCTS \/
    // ==================================================================================================== \/
public:
    struct Attribute {
        ArrayBuffer *arrayBuffer; // the ArrayBuffer to use
        int attributeID; // the attribute from that ArrayBuffer
        GLuint location;    // a location the in-attribute from a shader is bound to
        // more Attribute properties can be looked up in the ArrayBuffer (like the name)
    };

    // ========================================================================================================= \/
    // ============================================================================================ CONSTRUCTORS \/
    // ========================================================================================================= \/
public:
    VertexArrayObject() :
            mObjectName(0), mMode(GL_TRIANGLES),
                    mAttributes() {
        glGenVertexArrays(1, &mObjectName);
    }

    virtual ~VertexArrayObject(void) {
        // as always, OpenGL will ignore object name 0
        glDeleteVertexArrays(1, &mObjectName);
    }

    // ==================================================================================================== \/
    // ============================================================================================ GETTERS \/
    // ==================================================================================================== \/
public:
    inline GLuint getObjectName(void) const {
        return mObjectName;
    }
    inline GLenum getMode(void) const {
        return mMode;
    }

    // ==================================================================================================== \/
    // ============================================================================================ METHODS \/
    // ==================================================================================================== \/
public:
    inline void setMode(GLenum _mode) {
        mMode = _mode;
    }


    /**
     * Will set the attribute _arrayBufferAttribute of ArrayBuffer _arrayBuffer to the given attribute location.
     * If that location was already used it will get overwritten.
     * The _attributeLocation has to be lower than GL_MAX_VERTEX_ATTRIBS
     */
    void attachAttribute(ArrayBuffer *_arrayBuffer, int _arrayBufferAttribute, GLuint _attributeLocation) {
        Attribute newAttribute = { _arrayBuffer, _arrayBufferAttribute, _attributeLocation };
        attachAttribute(newAttribute);
    }

    void attachAttribute(const Attribute &_attribute) {

        GLint maxAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
        if(mAttributes.size() >= (uint32_t) maxAttributes) {
            std::cerr << "can't attach attribute "
                    << _attribute.arrayBuffer->getAttributes()[_attribute.attributeID].name
                    << " - maximum number of attributes reached: " << maxAttributes << std::endl;
            return;
        }

        mAttributes.push_back(_attribute);

        // query old VAO
        GLint oldVAO;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &oldVAO);

        bind(); // VAOs
        _attribute.arrayBuffer->bind(); // VBO
        glEnableVertexAttribArray(_attribute.location);
        ArrayBuffer::Attribute aba = _attribute.arrayBuffer->getAttributes()[_attribute.attributeID];
        glVertexAttribPointer(_attribute.location, aba.size, aba.type, aba.normalized,
                              _attribute.arrayBuffer->getStride(), (GLvoid*) aba.offset);

        // restore old VAO
        glBindVertexArray(oldVAO);
    }

    /**
     * Will detach the first found Attribute with the given attribute location.
     */
    void detachAttribute(GLuint _location) {
        for(AttributeVec::size_type i = 0; i < mAttributes.size(); ++i) {
            if(mAttributes[i].location == _location) {
                // the other pointer data is still set, but that isn't relevant if the attribute itself is deactivated
                glDisableVertexAttribArray(mAttributes[i].location);
                mAttributes.erase(mAttributes.begin() + i);
                return;
            }
        }
        // if we got here, no Attribute of the given name exists
        std::cerr << "can't detach attribute with location " << _location << " - no such Attribute" << std::endl;
    }

    /**
     * Will detach the first found Attribute with the given name.
     */
    void detachAttribute(const std::string &_name) {
        for(AttributeVec::size_type i = 0; i < mAttributes.size(); ++i) {
            if(mAttributes[i].arrayBuffer->getAttributes()[mAttributes[i].attributeID].name == _name) {
                // the other pointer data is still set, but that isn't relevant if the attribute itself is deactivated
                glDisableVertexAttribArray(mAttributes[i].location);
                mAttributes.erase(mAttributes.begin() + i);
                return;
            }
        }
        // if we got here, no attributes of the given name exists
        std::cerr << "can't detach attribute " << _name << " - no such Attribute" << std::endl;
    }

    void attachAllMatchingAttributes(ArrayBuffer *_arrayBuffer, ShaderProgram *_shaderProgram) {
        ArrayBuffer::AttributeVec attributes = _arrayBuffer->getAttributes();

        for(unsigned int a = 0; a < attributes.size(); ++a) {
            GLint attributeLocation = _shaderProgram->getAttributeLocation(attributes[a].name);
            if(attributeLocation != -1) {
                // found one!
                attachAttribute(_arrayBuffer, a, attributeLocation);
            } else {
                //std::cout << "attachAllMatchingAttributes - Note: attribute " << attributes[a].name
                //        << " does not exist in the shader" << std::endl;
            }
        }
        // now we could also query all attributes of the shader and see what we missed, but we will skip that!
    }

    void detachAllAttributes() {

        for(AttributeVec::const_iterator it = mAttributes.begin(),
                it_end = mAttributes.end(); it != it_end; ++it) {
            glDisableVertexAttribArray(it->location);
        }
        mAttributes.clear();
    }

    // ===================================================================================================== \/
    // ============================================================================================ WRAPPERS \/
    // ===================================================================================================== \/
public:
    //! Bind this VAO
    inline void bind(void) const {
        glBindVertexArray(mObjectName);
    }

    //! Nothing has to be prepared for a render call
    inline void render(void) const {
        bind();
        draw();
    }

    //! Will select the matching draw call. Remember to enable first!
    void draw(void) const {
        drawArrays();
    }

    //! Can be called directly instead of draw() iff the caller knows this is the correct call!
    inline void drawElements(void) const {
        //glDrawElements(mMode, mpElementArrayBuffer->getElements(), mpElementArrayBuffer->getType(), (GLvoid*)0);
    }

    //! Can be called directly instead of draw() iff the caller knows this is the correct call!
    inline void drawArrays(void) const {
        glDrawArrays(mMode, 0, mAttributes[0].arrayBuffer->getElements());
    }

    // ===================================================================================================== \/
    // ============================================================================================ TYPEDEFS \/
    // ===================================================================================================== \/
private:
    typedef std::vector<Attribute> AttributeVec;

    // =================================================================================================== \/
    // ============================================================================================ FIELDS \/
    // =================================================================================================== \/
private:
    GLuint mObjectName;          // OpenGL object name
    GLenum mMode;                // draw mode: GL_TRIANGLES etc.
    //ElementArrayBuffer      *mpElementArrayBuffer; // optional EAB
    AttributeVec mAttributes;          // vertex attributes
};

#endif // VERTEXARRAYOBJECT_HH
