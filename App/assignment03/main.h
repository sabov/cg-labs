#ifndef MAIN_H
#define MAIN_H


// OpenGL Math:
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

// draws a circle of radius 1.0 (screen is from -1..1) with a given transform and color (RGB)
void drawCircle( const glm::vec3 &color, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection );


extern bool arrowKeyUpPressed;
extern bool arrowKeyLeftPressed;
extern bool arrowKeyRightPressed;
extern bool arrowKeyDownPressed;

#endif
