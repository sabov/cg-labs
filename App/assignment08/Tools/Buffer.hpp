////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////

#ifndef OBJECTS_BUFFER_HH
#define OBJECTS_BUFFER_HH

/*
 * A generic OpenGL Buffer Object
 *
 * Mostly an OpenGL Buffer Wrapper: names of OpenGL calls are stripped of the
 * 'gl' and 'Buffer' tokens and setters got a 'set' prefix.
 *
 * Calls that give the target the buffer should get bound to have an alternative
 * call that uses the last used or set target.
 *
 * Note: Most methods will bind this buffer!
 */



#include <string>
#include <vector>

/*
 * A minimal(!) wrapper to allow multiple Buffer objects pointing to the same
 * OpenGL resource (like an ArrayBuffer and a TransformFeedbackBuffer).
 *
 * This has to be an extra object so all Buffer types can inherit from Buffer
 * below to allow a unified API.
 */
class BufferObject {
public:
    BufferObject() {
        glGenBuffers(1, &mObjectName);
    }

    ~BufferObject() {
        // buffer 0 will get ignored by OpenGL
        glDeleteBuffers(1, &mObjectName);
    }
    GLuint mObjectName;
};

/**
 * Buffers are general OpenGL Buffer Wrapper.
 * The OpenGL resource itself is attached via a shared pointer (GLBufferObject).
 * This was multiple Buffers can use internaly the same OpenGL resource, this is
 * useful if one resource should get interpreted as _different_ buffer types
 * so in that case the same GLBufferObject will get attached to different
 * Subclass Objects.
 *
 * Note: Subclasses should set the mTarget in there constructors!
 */
class Buffer {
    // ========================================================================================================= \/
    // ============================================================================================ CONSTRUCTORS \/
    // ========================================================================================================= \/
public:
    //! Most common default: a new Buffer corresponds to a new GL resource. You might decide on a binding point
    //! now or later.
    Buffer(GLenum _target) :
            mTarget(_target), mSize(0) {
        mBuffer = new BufferObject();
    }

    /**
     * Init with a given, external GL resource.
     *
     * Calling this with:
     * Buffer b( SharedGLBufferObject(NULL) );
     * Is the way to _explicitly_ state that a real OpenGL resource will get added later.
     * In this case no GL wrapper calls should ever get called until one gets set!
     */
    Buffer(BufferObject *_pBuffer, GLenum _target) :
            mTarget(_target), mBuffer(_pBuffer), mSize(0) {
    }

    virtual ~Buffer() {}

    // ==================================================================================================== \/
    // ============================================================================================ GETTERS \/
    // ==================================================================================================== \/
public:
    inline GLuint getObjectName(void) const {
        return mBuffer->mObjectName;
    }
    inline bool isValid(void) const {
        return (mBuffer && glIsBuffer(mBuffer->mObjectName));
    }
    inline BufferObject *getBufferObject() const {
        return mBuffer;
    }

    // ==================================================================================================== \/
    // ============================================================================================ SETTERS \/
    // ==================================================================================================== \/

    //! the GL buffer can get changed at any time
    void setBufferObject(BufferObject *_pBuffer) {
        mBuffer = _pBuffer;
    }

private:
    inline GLint getParameter(GLenum _parameter) {
        bind(mTarget);
        GLint value;
        glGetBufferParameteriv(mTarget, _parameter, &value);
        return value;
    }

    inline GLint64 getParameter64(GLenum _parameter) {
        bind(mTarget);
        GLint64 value;
        glGetBufferParameteri64v(mTarget, _parameter, &value);
        return value;
    }

    //! not side effect free! will bind this buffer to it's last target!
    //! caching of these values on RAM could be a good idea if needed very often!
    //inline GLint64   getSize()        { return             getParameter64( GL_BUFFER_SIZE         ); }
    inline GLint64 getMapOffset() {
        return getParameter64(GL_BUFFER_MAP_OFFSET);
    }
    inline GLint64 getMapLength() {
        return getParameter64(GL_BUFFER_MAP_LENGTH);
    }
    inline GLenum getUsage() {
        return (GLenum) getParameter(GL_BUFFER_USAGE);
    }
    inline GLenum getAccess() {
        return (GLenum) getParameter(GL_BUFFER_ACCESS);
    }
    inline GLint getAccessFlags() {
        return (GLint) getParameter(GL_BUFFER_ACCESS_FLAGS);
    }
    inline GLboolean isMapped() {
        return (GLboolean) getParameter(GL_BUFFER_MAPPED);
    }

    inline GLint64 getSize() {
        return mSize;
    }

    // ===================================================================================================== \/
    // ============================================================================================ WRAPPERS \/
    // ===================================================================================================== \/
public:
    //! Bind this buffer
    inline void bind(GLenum _target) {
        glBindBuffer(_target, mBuffer->mObjectName);
    }

    //! Bind this buffer to its target
    inline void bind() {
        glBindBuffer(mTarget, mBuffer->mObjectName);
    }

    //! Set data for this buffer. Use only to init the buffer!
    //! Note: The function is not const, because it changes the corresponding GPU data
    inline void setData(GLenum _target, GLsizeiptr _size, const GLvoid *_pData = NULL, GLenum _usage = GL_STATIC_DRAW) {
        mSize = _size;
        bind(_target);
        glBufferData(_target, _size, _pData, _usage);
    }

    //! Set data for this buffer at the last used target. Use only to init the buffer!
    inline void setData(GLsizeiptr _size, const GLvoid *_pData = NULL, GLenum _usage = GL_STATIC_DRAW) {
        setData(mTarget, _size, _pData, _usage);
    }

    //! Use this to modify the buffer
    inline void setSubData(GLenum _target, GLintptr _offset, GLsizeiptr _size, const GLvoid *_pData) {
        bind(_target);
        glBufferSubData(_target, _offset, _size, _pData);
    }

    //! Use this to modify the buffer
    inline void setSubData(GLintptr _offset, GLsizeiptr _size, const GLvoid *_pData) {
        setSubData(mTarget, _offset, _size, _pData);
    }

    /** Map a part of the buffer to client memory
     * _offset & _length are values in bytes relative to the buffer
     * _access must contain one (or both) of:
     *      GL_MAP_READ_BIT and GL_MAP_WRITE_BIT
     *  and optionally:
     *      GL_MAP_INVALIDATE_RANGE_BIT GL_MAP_INVALIDATE_BUFFER_BIT
     *      GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_UNSYNCHRONIZED_BIT
     */
    GLvoid *mapRange(GLenum _target, GLintptr _offset, GLsizeiptr _length, GLbitfield _access) {
        bind(_target);
        GLvoid *ret = glMapBufferRange(_target, _offset, _length, _access);
        return ret;
    }

    inline GLvoid *mapRange(GLintptr _offset, GLsizeiptr _length, GLbitfield _access) {
        return mapRange(mTarget, _offset, _length, _access);
    }

    /**
     * Spec:
     * If a buffer is mapped with the GL_MAP_FLUSH_EXPLICIT_BIT flag, modifications
     * to the mapped range may be indicated by calling this.
     * _offset and _length indicate a modified subrange of the mapping, in byte. The specified
     * subrange to flush is relative to the start of the currently mapped range of buffer.
     * This can be called multiple times to indicate distinct subranges
     * of the mapping which require flushing.
     */
    void flushMappedRange(GLenum _target, GLsizeiptr _offset, GLsizeiptr _length) {
        bind(_target);
        glFlushMappedBufferRange(_target, _offset, _length);
    }

    inline void flushMappedRange(GLintptr _offset, GLsizeiptr _length) {
        flushMappedRange(mTarget, _offset, _length);
    }

    //! Maps the whole buffer, if using GL 3+, better use mapRange!
    //! _access is GL_READ_ONLY GL_WRITE_ONLY or GL_READ_WRITE
    GLvoid *map(GLenum _target, GLenum _access) {
        bind(_target);
        GLvoid *ret = glMapBuffer(_target, _access);
        return ret;
    }
    inline GLvoid *map(GLenum _access) {
        return map(mTarget, _access);
    }

    GLboolean unmap(GLenum _target) {
        bind(_target);
        GLboolean ret = glUnmapBuffer(_target);
        return ret;
    }

    inline GLboolean unmap() {
        return unmap(mTarget);
    }

    /**
     * _target must be one of:
     GL_ARRAY_BUFFER
     GL_ATOMIC_COUNTER_BUFFER
     GL_COPY_READ_BUFFER
     GL_COPY_WRITE_BUFFER
     GL_DRAW_INDIRECT_BUFFER
     GL_ELEMENT_ARRAY_BUFFER
     GL_PIXEL_PACK_BUFFER
     GL_PIXEL_UNPACK_BUFFER
     GL_TEXTURE_BUFFER
     GL_TRANSFORM_FEEDBACK_BUFFER
     GL_UNIFORM_BUFFER
     * Can be changed at any time.
     *
     * Subclasses should overload this with a non-working function (+ a warning)
     * because an X-Buffer should not be attached _per default_ to Y!
     * Subclass buffers can however always use the method calls / binds with an
     * _explicit_ target (that doesn't match there one ones):
     *
     * XBuffer x;
     * x.bind( Y ); // ok, hope the programmer knowns what s/he does
     *
     * x.setTarget( Y ); // this is just calling for unintended side-effects!
     * x.bind();
     */
    virtual inline void setTarget(GLenum _target) {
        mTarget = _target;
    }

    // =================================================================================================== \/
    // ============================================================================================ FIELDS \/
    // =================================================================================================== \/
protected:
    GLenum mTarget;
    BufferObject *mBuffer;
    GLint64 mSize; // as this might get queried often (e.g. ArrayBuffer) we will explicitly mirror it in RAM)
};

#endif // ACGL_OPENGL_OBJECTS_BUFFER_HH
