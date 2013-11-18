#ifndef MAIN_H
#define MAIN_H

#include <gl_core_32.hh>

// OpenGL Math:
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif

// reads a textfile and returns the content as one string:
// if you need a const char** you can do something like:
//   std::string foo = getFileContent( "bar" );
//	 const char *foo_c = foo.c_str();
// 	 usePointerToCStringArray( &foo_c );
std::string getFileContent( const std::string &fileName );

// check for shader compile errors:
void printGLSLCompileLog( GLuint shaderHandle );

// check for shader link errors:
void printGLSLLinkLog( GLuint progHandle );

#endif
