#ifndef MAIN_H
#define MAIN_H


// OpenGL Math:
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tools/VertexArrayObject.hpp"
#include "Tools/ABReader.hpp"
#include "Tools/PNGReader.hpp"
#include "Tools/TextureData.hpp"
#include "Tools/HelperFunctions.hpp"
#include "Tools/GenericCamera.hh"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// Resources
extern ShaderProgram* g_shader;

extern GenericCamera g_camera;

extern VertexArrayObject* g_vaoBunny;
extern VertexArrayObject* g_vaoEarth;


glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far);

void resizeCallback(int newWidth, int newHeight);

void setupShaderUniforms(GLuint _program, int _scene);

void createShaderProgram(GLuint &vs, GLuint &fs, GLuint &prog, const std::string &vsFileName,
                         const std::string &fsFileName);

void printGLSLCompileLog(GLuint shaderHandle);
void printGLSLLinkLog(GLuint progHandle);
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, GLvoid *userParam);

#endif
