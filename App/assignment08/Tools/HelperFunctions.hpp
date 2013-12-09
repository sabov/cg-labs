/*
 * HelperFunctions.hpp
 *
 *  Created on: Nov 29, 2012
 *      Author: kremer
 */

#ifndef HELPERFUNCTIONS_HPP_
#define HELPERFUNCTIONS_HPP_

#include <gl_core_32.hh>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

inline GLint glTypeSize(GLenum _type) {
    switch(_type) {
        case GL_BYTE:
            return sizeof(GLubyte);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLbyte);
        case GL_SHORT:
            return sizeof(GLshort);
        case GL_UNSIGNED_SHORT:
            return sizeof(GLushort);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_DOUBLE:
            return sizeof(GLdouble);
    }
    return 0;
}

#endif /* HELPERFUNCTIONS_HPP_ */
