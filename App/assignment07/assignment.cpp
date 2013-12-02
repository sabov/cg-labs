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

      // ====================================================================
      // Add your code here:
      // Scene A
      // ====================================================================
      
      glUseProgram(prog_a);
      GLint projectionMatrixLocation = glGetUniformLocation( prog_a, "projectionMatrix" );
      GLint modelViewMatrixLocation  = glGetUniformLocation( prog_a, "modelViewMatrix" );




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


      // ====================================================================
      // End Exercise code
      // ====================================================================

      
    } else if(scene == 4) {

      // ====================================================================
      // Add your code here:
      // Scene D
      // ====================================================================






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
    createShaderProgram(vs_a, fs_a, prog_a, "shader_solution_a.vsh", "shader_solution_a.fsh");
    createShaderProgram(vs_b, fs_b, prog_b, "shader_solution_b.vsh", "shader_solution_b.fsh");
    createShaderProgram(vs_c, fs_c, prog_c, "shader_solution_c.vsh", "shader_solution_c.fsh");

    
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





    // ====================================================================
    // Add your code here:
    // Generate VertexArrayObjects for the scenes
    // Scene B
    // ====================================================================




    // ====================================================================
    // Add your code here:
    // Generate VertexArrayObjects for the scenes
    // Scene C
    // ====================================================================






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
}
