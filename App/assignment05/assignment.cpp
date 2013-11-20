/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.h"
using namespace std;

extern int g_numberOfBunnyVertices;
extern int g_bunnyStrideSize;
extern float g_bunnyMesh[];
extern int g_bunnyColorStrideSize;
extern unsigned char g_bunnyColor[];

// add your team members names and matrikel numbers here:
void printStudents()
{
	cout << "Student Name 0, matrikel number 0" << endl;
	cout << "Student Name 1, matrikel number 1" << endl;
	cout << "Student Name 2, matrikel number 2" << endl;
	cout << "Student Name 3, matrikel number 3" << endl;
}

glm::mat4 g_ModelViewMatrix;
glm::mat4 g_ProjectionMatrix;

glm::mat4 buildFrustum( float phiInDegree, float aspectRatio, float near, float far) {

    float phiHalfInRadians = 0.5f*phiInDegree * (M_PI/180.0f);
    float t = near * tan( phiHalfInRadians );
    float b = -t;
    float left = b*aspectRatio;
    float right = t*aspectRatio;

	return glm::frustum( left, right, b, t, near, far );
}

void resizeCallback( int newWidth, int newHeight )
{
	// define the part of the screen OpenGL should draw to (in pixels):
    glViewport( 0, 0, newWidth, newHeight );
    g_ProjectionMatrix = buildFrustum(90.0f, (newWidth/(float)newHeight), 0.1f, 100.0f);
}


// ==============================================
// Global varibles for binding the vbo
// ==============================================
GLuint bunny_vbo;
GLuint bunny_color_vbo;

// ==============================================
// Global varibles for the shaders.
// ==============================================
GLuint vs_a, fs_a, prog_a;
GLuint vs_b, fs_b, prog_b;
GLuint vs_c, fs_c, prog_c;

void drawScene(int scene, float runTime) {
	
	// glm::lookAt needs the eye position, the center to look at and the up vector, so it works a bit different 
	// than our lookAt from last week:
	glm::vec3 pos = glm::vec3(1.5f*cos(runTime), 0.0f, 1.5f*sin(runTime) );
	g_ModelViewMatrix = glm::lookAt( pos, glm::vec3(0,0,0), glm::vec3(0,1,0) );
	glm::mat4 modelMatrix = glm::translate( 0.0f, 0.0f, 0.35f );
	g_ModelViewMatrix = g_ModelViewMatrix * modelMatrix;
	
	if (scene == 1) {

        // ====================================================================
        // assignment part a
        // Add your code here:
        // ====================================================================

        glDrawArrays( GL_TRIANGLES, 0, g_numberOfBunnyVertices);

        // ====================================================================
        // End Exercise code
        // ====================================================================

	} else if (scene == 2) {
		
        // ====================================================================
        // assignment part b
        // Add your code here:
        // ====================================================================

        // ====================================================================
        // End Exercise code
        // ====================================================================


	} else if (scene == 3) {
		
        // ====================================================================
        // assignment part c
        // Add your code here:
        // ====================================================================


        // ====================================================================
        // End Exercise code
        // ====================================================================
	}
}

void createShaderProgram( GLuint &vs, GLuint &fs, GLuint &prog, const std::string &vsFileName, const std::string &fsFileName ) {

    // =========================================================================
    // create your shaders here
    //
    // Input are the filenames of the vertex and the fragment shader.
    //
    // You have to return the handles of the vertex shader, fragment shader
    // and the final program.
    //
    // Code required for a,b,c
    //
    // Add your code here:
    // ====================================================================

    std::string vsFile = getFileContent(vsFileName);
    std::string fsFile = getFileContent(fsFileName);
    const char *vsrc = vsFile.c_str();
    const char *fsrc = fsFile.c_str();

    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    prog = glCreateProgram();

    glShaderSource( vs, 1, &vsrc, NULL );
    glCompileShader( vs );
    glShaderSource( fs, 1, &fsrc, NULL );
    glCompileShader( fs );

    glAttachShader( prog, vs );
    glAttachShader( prog, fs );

    glLinkProgram( prog );

    // the shaders are now ready to use:
    glUseProgram( prog );

    glEnableVertexAttribArray(0); // now were ready to draw
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, g_bunnyStrideSize, 0 );

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

void initCustomResources()
{

    // =========================================================================
    // create your ressources here, e.g. buffers,...
    //
    // Code required for a,b,c
    //
    // Add your code here:
    // ====================================================================


    glGenBuffers( 1, &bunny_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, bunny_vbo );
    glBufferData( GL_ARRAY_BUFFER, g_numberOfBunnyVertices * g_bunnyStrideSize, g_bunnyMesh,
        GL_STATIC_DRAW);

    // ====================================================================
    // End Exercise code
    // ====================================================================


    // create the shaders:
    createShaderProgram( vs_a, fs_a, prog_a, "shader_340311_a.vsh", "shader_340311_a.fsh" );
    //createShaderProgram( vs_b, fs_b, prog_b, "shader_340311_b.vsh", "shader_340311_b.fsh" );
    //createShaderProgram( vs_c, fs_c, prog_c, "shader_340311_c.vsh", "shader_340311_c.fsh" );
}

void deleteCustomResources()
{


    // ====================================================================
    // don't forget to delete your OpenGL ressources (shaders, buffers, etc.)!
    // Add your code here:
    // ====================================================================

    glDeleteProgram(prog_a);
    glDeleteShader(vs_a);
    glDeleteShader(fs_a);

    glDeleteBuffers(1, &bunny_vbo);


    // ====================================================================
    // End Exercise code
    // ====================================================================
}

