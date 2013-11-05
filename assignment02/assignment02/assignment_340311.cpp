/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.h"
#include <unistd.h>
#define PI 3.14159265
using namespace std;

// add your team members names and matrikel numbers here:
void printStudents()
{
	cout << "Student Tarasenko Petro, matrikel number 340826" << endl;
	cout << "Student Oleksandr Sabov, matrikel number 340311" << endl;
	cout << "Student Chih-Yun Tsai, matrikel number 328782" << endl;
}



glm::mat4 glm_scale(float x, float y, float z) {
	float scaleMatrix [16] = {x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1};
	return glm::make_mat4(scaleMatrix);
}

glm::mat4 glm_translate(float x, float y, float z) {
	float translateMatrix [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, 0, 0, 1};
	return glm::make_mat4(translateMatrix);
}

glm::mat4 glm_rotate(float a, glm::vec3 vector) {
    a = a * PI / 180;
	float rotationMatrixZ [16] = {cos(a), sin(a), 0, 0, -sin(a), cos(a), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	return glm::make_mat4(rotationMatrixZ);
}

glm::mat4 setScale(float scale) {
    return glm_scale(scale, scale, 1.0f);
}

float t = 0;

void drawScene(int scene, float runTime) {
    int i;
    float step;
    //colors
    glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 darkGrey = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 grey = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 blue = glm::vec3(0.1f, 0.1f, 0.6f);
    glm::vec3 red = glm::vec3(0.6f, 0.1f, 0.1f);
    glm::vec3 yellow = glm::vec3(0.6f, 0.6f, 0.1f);

    //draw stand
    drawCircle(grey, glm_scale(0.05f, 0.7f, 1.0f) * glm_translate(-18.5f, 0.0f, 0.0f));

    //draw track
    drawCircle(blue, setScale(0.85f));
    drawCircle(darkGrey, setScale(0.84f));
    drawCircle(blue, setScale(0.68f));
    drawCircle(black, setScale(0.67f));

    //draw start-finish
    glm::mat4 finishS = glm_scale(0.006f, 0.03f, 1.0f);
    for(i = 0; i < 9; i++) {
        step = i * 3.0f;
        drawCircle(white, finishS * glm_translate(-139.0f + step, 0.0f, 0.0f));
    }

    //lines
    glm::mat4 lineS = glm_scale(0.01f, 0.03f, 1.0f);
    for(i = 1; i < 37; i++) {
        drawCircle(white, glm_rotate(i * 12.0f - 6.0f, glm::vec3(0,0,1)) * lineS * glm_translate(-76.0f, 0.0f, 0.0f));
    }

    //cars
    glm::mat4 carsS = glm_scale(0.02f, 0.1f, 1.0f);
    if(t > 360) {
        t = 0;
    }
    drawCircle(yellow, glm_rotate(t, glm::vec3(0,0,1)) * carsS * glm_translate(-35.5f, 0.0f, 0.0f));
    drawCircle(red, glm_rotate(t*2, glm::vec3(0,0,1)) * carsS * glm_translate(-40.0f, 0.0f, 0.0f));
    t++;
	// otherwise in ubuntu it runs very fast
    usleep(10000);
}



void initCustomResources()
{
}

void deleteCustomResources()
{
}

