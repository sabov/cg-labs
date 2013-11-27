/*
 * Basics of Computer Graphics Exercise
 */

#include "assignment.h"
using namespace std;

extern GLuint teapot_vbo;
extern GLuint teapot_ea;

GLuint vs_a, fs_a, prog_a;
GLuint vs_b, fs_b, prog_b;
GLuint vs_c, fs_c, prog_c;

// Precompute inverse transposed model-view matrix
glm::mat4 invTranspModelView;

extern glm::mat4 g_ModelViewMatrix;
extern glm::mat4 g_ProjectionMatrix;

// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Name 0, matrikel number 0" << endl;
    cout << "Student Name 1, matrikel number 1" << endl;
    cout << "Student Name 2, matrikel number 2" << endl;
    cout << "Student Name 3, matrikel number 3" << endl;
}

void drawScene(int scene, float runTime) {

    // Create modelview matrix
    // First: Create transformation into eye-space
    // glm::lookAt needs the eye position, the center to look at and the up vector
    g_ModelViewMatrix = glm::lookAt(eyePos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    // One round should take 10 seconds
    float angle = runTime * 360.0f / 10.0f;

    glm::mat4 modelMatrix = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix *= glm::scale(1.0f, 1.3f, 1.0f);

    g_ModelViewMatrix = g_ModelViewMatrix * modelMatrix;
    
    if(scene == 1) {

        glUseProgram(prog_a);
        setupShaderUniforms(prog_a, scene);

    } else if(scene == 2) {

        glUseProgram(prog_b);
        setupShaderUniforms(prog_b, scene);

    } else if(scene == 3) {

        glUseProgram(prog_c);
        setupShaderUniforms(prog_c, scene);
    }
    
    // Draw teapot
    glDrawElements(GL_TRIANGLES, g_numberOfTeapotTriangles*3, GL_UNSIGNED_INT, (void*)0);
}

void initCustomResources() {
    // create your ressources here, e.g. shaders, buffers,...

    // create the shaders:
    createShaderProgram(vs_a, fs_a, prog_a, "shader_123456_a.vsh", "shader_123456_a.fsh");
    createShaderProgram(vs_b, fs_b, prog_b, "shader_123456_b.vsh", "shader_123456_b.fsh");
    createShaderProgram(vs_c, fs_c, prog_c, "shader_123456_c.vsh", "shader_123456_c.fsh");
}

void deleteCustomResources() {
    // don't forget to delete your OpenGL ressources (shaders, buffers, etc.)!

    glDeleteShader(vs_a);
    glDeleteShader(vs_b);
    glDeleteShader(vs_c);
    glDeleteShader(fs_a);
    glDeleteShader(fs_b);
    glDeleteShader(fs_c);
    glDeleteProgram(prog_a);
    glDeleteProgram(prog_b);
    glDeleteProgram(prog_c);
}

