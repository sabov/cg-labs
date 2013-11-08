#ifndef MAIN_H
#define MAIN_H

// OpenGL Math:
#define GLM_SWIZZLE_RGBA
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

void setPixel( int x, int y, glm::vec3 color );

#endif
