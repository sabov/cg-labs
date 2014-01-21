#ifndef MAIN_H
#define MAIN_H


// OpenGL Math:
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tools/VertexArrayObject.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif

extern const float g_phi;
extern int g_windowHeight;
extern int g_windowWidth;
extern const float g_near;
extern const float g_far;

extern glm::mat4 g_modelViewMatrix;
extern glm::mat4 g_projectionMatrix;

// Resources
extern ShaderProgram* g_simplePointShader;
extern ShaderProgram* g_ssSplatShader;
extern ShaderProgram* g_osSplatShader;
extern ShaderProgram* g_osBlendSplatShader;

const glm::vec3 eyePos1 = glm::vec3(0.0f, 1.0f, 6.0f);
const glm::vec3 eyePos2 = glm::vec3(-1.5f, 1.0f, -2.0f);
const glm::vec3 viewDir2 = glm::vec3(1.0f, 0.0f, 0.0f);

glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far);

void resizeCallback(int newWidth, int newHeight);

void printGLSLCompileLog(GLuint shaderHandle);
void printGLSLLinkLog(GLuint progHandle);
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, GLvoid *userParam);

#endif
