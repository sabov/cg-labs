/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.h"
using namespace std;

// add your team members names and matrikel numbers here:
void printStudents()
{
	cout << "Student Name 0, matrikel number 0" << endl;
	cout << "Student Name 1, matrikel number 1" << endl;
	cout << "Student Name 2, matrikel number 2" << endl;
	cout << "Student Name 3, matrikel number 3" << endl;
}

glm::mat4 setScale(float scale) {
    return glm::scale(scale, scale, 1.0f);
}
float t = 0;
float s = 0.2f;
float scaleMatrix [16] = {s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1};
float a = 45.0f;
float x = -10.5f;
float rotationMatrix [16] = {1, 0, 0, 0, cos(a), -sin(a), 0, 0, sin(a), cos(a), 0, 0, 0, 0, 1};
float rotationMatrixZ [16] = {cos(a), sin(a), 0, 0, -sin(a), cos(a), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
float translateMatrix [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, 0, 0, 1};
glm::mat4 scale = glm::make_mat4(scaleMatrix);
glm::mat4 rotate1 = glm::make_mat4(rotationMatrixZ);
glm::mat4 translate = glm::make_mat4(translateMatrix);

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
    drawCircle(grey, glm::scale(0.05f, 0.7f, 1.0f) * glm::translate(-18.5f, 0.0f, 0.0f));

    //draw track
    drawCircle(blue, setScale(0.85f));
    drawCircle(darkGrey, setScale(0.84f));
    drawCircle(blue, setScale(0.68f));
    drawCircle(black, setScale(0.67f));

    //draw start-finish
    glm::mat4 finishS = glm::scale(0.006f, 0.03f, 1.0f);
    for(i = 0; i < 9; i++) {
        step = i * 3.0f;
        drawCircle(white, finishS * glm::translate(-139.0f + step, 0.0f, 0.0f));
    }

    //lines
    glm::mat4 lineS = glm::scale(0.01f, 0.03f, 1.0f);
    for(i = 1; i < 36; i++) {
        drawCircle(white, glm::rotate(i * 10.0f, glm::vec3(0,0,1)) * lineS * glm::translate(-76.0f, 0.0f, 0.0f));
    }

    //cars
    glm::mat4 carsS = glm::scale(0.02f, 0.1f, 1.0f);
    if(t > 360) {
        t = 0;
    }
    drawCircle(yellow, glm::rotate(t*2, glm::vec3(0,0,1)) * carsS * glm::translate(-35.5f, 0.0f, 0.0f));
    drawCircle(red, glm::rotate(t, glm::vec3(0,0,1)) * carsS * glm::translate(-40.0f, 0.0f, 0.0f));
    t++;

    drawCircle(blue, rotate1 * carsS * translate);

    //drawCircle( glm::vec3(1.0f, 0.0f, 0.0f), glm::rotate( 85.0f, glm::vec3(0,0,1)) * glm::scale(0.1f,0.5f, 1.0f) * glm::translate(0.5f,0.5f, 0.0f));
}



void initCustomResources()
{
}

void deleteCustomResources()
{
}

