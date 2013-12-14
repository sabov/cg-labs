////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////

#ifndef ARRAYBUFFER_HH
#define ARRAYBUFFER_HH

/*
 * An ArrayBuffer holds an array of per-vertex data. In its simplest form an
 * array of one attribute, for example the vertex position or texture-coordinate.
 * An ArrayBuffer however can also hold multiple attributes in an interleaved
 * way.
 *
 * An ArrayBuffer can be drawn directly or indexed in combination with an
 * ElementArrayBuffer.
 *
 * The combination of (multiple) attributes of (multiple) ArrayBuffers
 * and one (optional) ElementArrayBuffer is a VertexBufferObject or VertexArrayObject.
 *
 * Note: In some documents ArrayBuffers (and sometimes ElementArrayBuffers) are
 *       called VertexBufferObjects, VBOs. The original extension that introduced
 *       these two new buffer types was called ARB_vertex_buffer_object but the buffers
 *       itself are called ArrayBuffer and ElementArrayBuffer.
 */

#include <gl_core_32.hh>

#include <string>
#include <vector>

#include "HelperFunctions.hpp"

#include "Buffer.hpp"

class ArrayBuffer : public Buffer {
    // ==================================================================================================== \/
    // ============================================================================================ STRUCTS \/
    // ==================================================================================================== \/
public:
    //! Each attribute has a size (#components, e.g. normal with x/y/z => 3) and an offset in the stride (in bytes)
    struct Attribute {
        std::string name;       // human readable name, can be used to match the attribute to shader programs
        GLenum type;       // GL_FLOAT, GL_UNSIGNED_BYTE etc.
        GLint size;       // #elements per attribute
        size_t offset;     // offset in bytes into the array
        GLboolean normalized; // int types can get normalzed to 0..1 / -1..1 by GL
    };

    // ===================================================================================================== \/
    // ============================================================================================ TYPEDEFS \/
    // ===================================================================================================== \/
public:
    typedef std::vector<Attribute> AttributeVec;

    // ========================================================================================================= \/
    // ============================================================================================ CONSTRUCTORS \/
    // ========================================================================================================= \/
public:
    ArrayBuffer() :
            Buffer(GL_ARRAY_BUFFER), mStride(0), mAttributes() {
    }

    virtual ~ArrayBuffer() {}

    // ==================================================================================================== \/
    // ============================================================================================ GETTERS \/
    // ==================================================================================================== \/
public:
    inline GLsizei getElements(void) const {
        return mSize / mStride;
    }
    inline GLsizei getStride(void) const {
        return mStride;
    }
    inline const AttributeVec& getAttributes(void) const {
        return mAttributes;
    }

    // ==================================================================================================== \/
    // ============================================================================================ METHODS \/
    // ==================================================================================================== \/
public:

    /* Attributes:
     *
     * _type is the GL type
     * _size the number of elements in this attribute (1..4)
     * _normalized is the attribute normalization for int types
     *
     * Want to add tightly packed attributes in order?
     *  -> use defineAttribute()
     *
     * Want to add attributes with individual padding in order?
     *  -> use defineAttributeWithPadding()
     *
     * Want to add attributes out-of-order?
     *  -> use defineAttributeWithOffset()
     *
     * The stride size gets always set to the minimal stride size that covers all defined attributes (/w padding).
     * All define methods can get mixed!
     *
     *
     * ab->defineAttribute(            "pos",       GL_FLOAT, 3    ); // stride: 12 bytes
     * ab->defineAttributeWithPadding( "color",     GL_CHAR,  3, 1 ); // stride: 12 + 3 + 1 = 16 bytes
     * ab->defineAttributeWithOffset(  "colorNorm", GL_CHAR,  3, 12, GL_TRUE ); // stride is still 16 as 12+3 <= 16!
     */
    //! Adds the attribute at the end of the existing attributes, stride gets computed automatically
    void defineAttribute(const std::string& _name, GLenum _type, GLint _size, GLboolean _normalized = GL_FALSE) {
        GLuint offset = mStride;

        Attribute attribute = { _name, _type, _size, offset, _normalized };
        defineAttribute(attribute);
    }

    //! Adds the attribute at the end of the existing attributes, stride gets computed automatically
    //! + extra padding in bytes
    void defineAttributeWithPadding(const std::string& _name, GLenum _type, GLint _size, GLuint _padding,
                                    GLboolean _normalized = GL_FALSE) {
        Attribute attribute = { _name, _type, _size, (GLuint) mStride, _normalized };
        defineAttribute(attribute);
        // defineAttribute will shift the mStride to the end of this attribute, so we only have to
        // add the explicit padding:
        mStride += _padding;
    }

    //! Adds an attribute defined by an offset: this way an attribute can get added at arbitrary
    //! locations in the stride. If it's added at the end, the stride gets resized.
    void defineAttributeWithOffset(const std::string& _name, GLenum _type, GLint _size, GLuint _offset,
                                   GLboolean _normalized = GL_FALSE) {
        Attribute attribute = { _name, _type, _size, _offset, _normalized };
        defineAttribute(attribute);
    }

    //! Takes care of a valid stride size and adds the attribute
    void defineAttribute(const Attribute &_attribute) {
        // this way attribute definitions don't have to be in order!
        mStride = std::max((GLsizei) _attribute.offset + glTypeSize(_attribute.type) * _attribute.size, mStride);
        mAttributes.push_back(_attribute);
    }

    //! Setting of the stride size explicitly is not needed if the attributes are defined correctly (with padding)
    inline void setStride(GLsizei _stride) {
        mStride = _stride;
    }

    //! removes all attributes
    inline void removeAttributes(void) {
        mStride = 0;
        mAttributes.clear();
    }

    //! Overloaded from the base class to _prevent_ redefining of the binding target! (see Buffer)
    inline void setTarget(GLenum) {
        std::cerr << "DON'T redefine the target binding point of an ArrayBuffer" << std::endl;
    }

    // =================================================================================================== \/
    // ============================================================================================ FIELDS \/
    // =================================================================================================== \/
protected:
    GLsizei mStride;
    AttributeVec mAttributes;
};

#endif
