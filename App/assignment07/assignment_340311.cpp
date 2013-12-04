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

// vertex data as defined in vertexData.cpp:
// number of triangles:
int trianglesInVBOs = 1984;

//// per vertex: x,y,z
extern float spherePosition[];

//// per vertex: nx,ny,nz, cr,cg,cb
extern float sphereNormalColor[];

//// per vertex: x,y,z, nx,ny,nz, cr,cg,cb
extern float konus[];


GLuint vboSpherePos;
GLuint vboSphereNormalColor;
GLuint vboKonus;

GLuint vaoA, vaoB, vaoC;


// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Tarasenko Petro, matrikel number 340826" << endl;
    cout << "Student Oleksandr Sabov, matrikel number 340311" << endl;
    cout << "Student Chih-Yun Tsai, matrikel number 328782" << endl;   
}

void drawScene(int scene, float runTime) {

    // Create modelview matrix
    // First: Create transformation into eye-space
    // glm::lookAt needs the eye position, the center to look at and the up vector
    g_ModelViewMatrix = glm::lookAt(eyePos	, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    // One round should take 10 seconds
    float angle = runTime * 360.0f / 10.0f;

    glm::mat4 modelMatrix = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix *= glm::scale(1.0f, 1.3f, 1.0f);

    glm::mat4 g_ModelViewMatrixWithoutRotation = g_ModelViewMatrix * glm::scale(1.0f, 1.3f, 1.0f);
    g_ModelViewMatrix = g_ModelViewMatrix * modelMatrix;

    if(scene == 1) {

      // ====================================================================
      // Add your code here:
      // Scene A
      // ====================================================================

      glUseProgram(prog_a);
      GLint projectionMatrixLocation = glGetUniformLocation( prog_a, "projectionMatrix" );
      GLint modelViewMatrixLocation = glGetUniformLocation( prog_a, "modelViewMatrix" );
      glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ProjectionMatrix));
      glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrix));
      glBindVertexArray(vaoA);
      glDrawArrays( GL_TRIANGLES, 0, trianglesInVBOs*3);

      // ====================================================================
      // End Exercise code
      // ====================================================================


    } else if(scene == 2) {
      // ====================================================================
      // Add your code here:
      // Scene B
      // ====================================================================

      glUseProgram(prog_b);
      GLint projectionMatrixLocation = glGetUniformLocation( prog_b, "projectionMatrix" );
      GLint modelViewMatrixLocation  = glGetUniformLocation( prog_b, "modelViewMatrix" );
      GLint timeLocation             = glGetUniformLocation( prog_b, "time" );
      glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ProjectionMatrix));
      glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrix));
      glUniform1f(timeLocation, runTime);
      glBindVertexArray(vaoB);
      glDrawArrays( GL_TRIANGLES, 0, trianglesInVBOs * 3);

      // ====================================================================
      // End Exercise code
      // ====================================================================

    } else if(scene == 3) {

      // ====================================================================
      // Add your code here:
      // Scene C
      // ====================================================================

      glUseProgram(prog_c);
      GLint projectionMatrixLocation = glGetUniformLocation( prog_c, "projectionMatrix" );
      GLint modelViewMatrixLocation  = glGetUniformLocation( prog_c, "modelViewMatrix" );
      GLint timeLocation             = glGetUniformLocation( prog_c, "time" );
      glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ProjectionMatrix));
      glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrix));
      glUniform1f(timeLocation, runTime);
      glBindVertexArray(vaoC);
      glDrawArrays( GL_TRIANGLES, 0, trianglesInVBOs * 3);


      // ====================================================================
      // End Exercise code
      // ====================================================================

    } else if(scene == 4) {

      // ====================================================================
      // Add your code here:
      // Scene D
      // ====================================================================


      glUseProgram(prog_c);

      g_ModelViewMatrixWithoutRotation = g_ModelViewMatrixWithoutRotation * glm::translate(2.0f, 0.0f, 0.0f);	
      GLint projectionMatrixLocation = glGetUniformLocation( prog_c, "projectionMatrix" );
      GLint modelViewMatrixLocation  = glGetUniformLocation( prog_c, "modelViewMatrix" );
      GLint timeLocation             = glGetUniformLocation( prog_c, "time" );
      glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ProjectionMatrix));
      glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrixWithoutRotation));
      glUniform1f(timeLocation, runTime);
      glBindVertexArray(vaoC);
      glDrawArrays( GL_TRIANGLES, 0, trianglesInVBOs * 3);

      glUseProgram(prog_b);
      g_ModelViewMatrixWithoutRotation = g_ModelViewMatrixWithoutRotation * glm::translate(-4.0f, 0.0f, 0.0f);
      glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrixWithoutRotation));
      glUniform1f(timeLocation, runTime);
      glBindVertexArray(vaoB);
      glDrawArrays( GL_TRIANGLES, 0, trianglesInVBOs * 3);

      // ====================================================================
      // End Exercise code
      // ====================================================================

    }
}

void initCustomResources() {

    // ====================================================================
    // create your ressources here, e.g. shaders, buffers,...
    // ====================================================================


    // create the shaders:
    createShaderProgram(vs_a, fs_a, prog_a, "shader_340311_a.vsh", "shader_340311_a.fsh");
    createShaderProgram(vs_b, fs_b, prog_b, "shader_340311_b.vsh", "shader_340311_b.fsh");
    createShaderProgram(vs_c, fs_c, prog_c, "shader_340311_c.vsh", "shader_340311_c.fsh");

    // ====================================================================
    // VBOs:
    // ====================================================================
    glGenBuffers( 1, &vboSpherePos );
    glBindBuffer( GL_ARRAY_BUFFER, vboSpherePos );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float)*trianglesInVBOs*3*3, // 3 vertices per triangle, 3 floats per vertex
                  spherePosition, GL_STATIC_DRAW );

    glGenBuffers( 1, &vboSphereNormalColor);
    glBindBuffer( GL_ARRAY_BUFFER, vboSphereNormalColor );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float)*trianglesInVBOs*3*6, // 3 vertices per triangle, 6 floats per vertex
                 sphereNormalColor, GL_STATIC_DRAW );

    glGenBuffers( 1, &vboKonus );
    glBindBuffer( GL_ARRAY_BUFFER, vboKonus );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float)*trianglesInVBOs*3*9, // 3 vertices per triangle, 9 floats per vertex
                 konus, GL_STATIC_DRAW );


    // ====================================================================
    // Add your code here:
    // Generate VertexArrayObjects for the scenes
    // Scene A
    // ====================================================================

    glGenVertexArrays(1, &vaoA);
    glBindVertexArray(vaoA);

    glBindBuffer( GL_ARRAY_BUFFER, vboSpherePos );
    GLint posAttrib_a = glGetAttribLocation( prog_a, "position" );
    glEnableVertexAttribArray(posAttrib_a);
    glVertexAttribPointer(posAttrib_a, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindVertexArray(0);


    // ====================================================================
    // Add your code here:
    // Generate VertexArrayObjects for the scenes
    // Scene B
    // ====================================================================

     glGenVertexArrays(1, &vaoB);
     glBindVertexArray(vaoB);

     glBindBuffer( GL_ARRAY_BUFFER, vboSpherePos );
     GLint posAttrib_b = glGetAttribLocation( prog_b, "position" );
     glEnableVertexAttribArray(posAttrib_b);
     glVertexAttribPointer(posAttrib_b, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

     glBindBuffer( GL_ARRAY_BUFFER, vboSphereNormalColor );
     GLint colAttrib_b = glGetAttribLocation( prog_b, "color" );
     glEnableVertexAttribArray(colAttrib_b);
     glVertexAttribPointer(colAttrib_b, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

     glBindVertexArray(0);
     
     
    // ====================================================================
    // Add your code here:
    // Generate VertexArrayObjects for the scenes
    // Scene C
    // ====================================================================


     glGenVertexArrays(1, &vaoC);
     glBindVertexArray(vaoC);

     glBindBuffer( GL_ARRAY_BUFFER, vboSpherePos );
     GLint posAttrib_cs = glGetAttribLocation( prog_c, "positionS" );
     glEnableVertexAttribArray(posAttrib_cs);
     glVertexAttribPointer(posAttrib_cs, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

     glBindBuffer( GL_ARRAY_BUFFER, vboSphereNormalColor );
     GLint colAttrib_cs = glGetAttribLocation( prog_c, "colorS" );
     glEnableVertexAttribArray(colAttrib_cs);
     glVertexAttribPointer(colAttrib_cs, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

     glBindBuffer( GL_ARRAY_BUFFER, vboKonus );
     GLint posAttrib_ck = glGetAttribLocation( prog_c, "positionK" );
     glEnableVertexAttribArray(posAttrib_ck);
     glVertexAttribPointer(posAttrib_ck, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), 0);

     GLint colAttrib_ck = glGetAttribLocation( prog_c, "colorK" );
     glEnableVertexAttribArray(colAttrib_ck);
     glVertexAttribPointer(colAttrib_ck, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(6*sizeof(float)));

     glBindVertexArray(0);


    
    // ====================================================================
    // End Exercise code
    // ====================================================================


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

    glDeleteBuffers(1, &vboSpherePos);
    glDeleteBuffers(1, &vboSphereNormalColor);
    glDeleteBuffers(1, &vboKonus);
    glDeleteBuffers(1, &vaoA);
    glDeleteBuffers(1, &vaoB);
    glDeleteBuffers(1, &vaoC);
}

