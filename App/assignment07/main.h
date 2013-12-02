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

extern int g_numberOfTeapotVertices;
extern int g_numberOfTeapotTriangles;
extern int g_teapotVertexStrideSize;
extern int g_teapotElementStrideSize;

extern float g_teapotVertices[];
extern unsigned int g_teapotElements[];

extern glm::mat4 g_ModelViewMatrix;
extern glm::mat4 g_ProjectionMatrix;

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

glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far);

void resizeCallback(int newWidth, int newHeight);

void setupShaderUniforms(GLuint _program, int _scene);

void createShaderProgram(GLuint &vs, GLuint &fs, GLuint &prog, const std::string &vsFileName,
                         const std::string &fsFileName);

#endif
