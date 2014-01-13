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

extern glm::mat4 g_ModelViewMatrix;
extern glm::mat4 g_ProjectionMatrix;
extern glm::mat4 g_invTranspModelView;
extern bool g_bunnyVisibility[16];

void drawBunny( int number );
void drawBoundingObject( int number );

#endif
