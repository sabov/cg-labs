////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013, Computer Graphics Group RWTH Aachen University         //
// All rights reserved.                                                       //
////////////////////////////////////////////////////////////////////////////////
/*
 * Basics of Computer Graphics Exercise
 *
 * DO NOT EDIT THIS FILE!
 */

#include <gl_core_32.hh>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <limits>

#include "assignment.h"

// OpenGL Math:
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// some variables we'll need:
//
unsigned int windowWidth  = 512;
unsigned int windowHeight = 512;
float nearPlane =   0.1f;
float farPlane  = 100.0f;
GLFWwindow* g_window;

// assigment specific includes:

// assignment specific variables:
GLuint teapot_vbo;
GLuint teapot_ea;
GLuint vao;

glm::mat4 g_ModelViewMatrix;
glm::mat4 g_ProjectionMatrix;


//
// Can be usefull if your hardware supports ARB_debug_output. If you have compile problems with this,
// just comment it out.
//
void debugCallback(GLenum source, GLenum type, GLuint id,
                   GLenum severity, GLsizei length,
                   const GLchar *message, const void *userParam)
{
     cout << "Note: ";
     if (source == GL_DEBUG_SOURCE_API_ARB)
            cout << "OpenGL";
     else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
            cout << "your OS";
     else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
            cout << "the Shader Compiler";
     else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
            cout << "a third party component";
     else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
            cout << "your application";
     else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
            cout << "someone";

     cout << " reported a problem - it's a";
     if (type == GL_DEBUG_TYPE_ERROR_ARB)
            cout << "n error";
     else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
            cout << " deprecated behavior";
     else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
            cout << "n undefined behavior";
     else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
            cout << " portability issue";
     else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
            cout << " performance issue";
     else if(type == GL_DEBUG_TYPE_OTHER_ARB)
            cout << " something";

     cout << endl;
     cout << "The message was: " << message << endl << endl;
}



/**********************************************************************************************************************
 * Returns true if a window with the desired context could get created.
 * Requested OpenGL version gets set by ACGL defines.
 */
bool createWindow( bool forceOpenGL32 )
{
    /////////////////////////////////////////////////////////////////////////////////////
    // Configure OpenGL context
    //
	if (forceOpenGL32) {
		// request OpenGL 3.2, will return a 4.1 context on Mavericks as well
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}

    // activate multisampling (second parameter is the number of samples):
    //glfwWindowHint( GLFW_SAMPLES, 8 );

    // request an OpenGL debug context:
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

    // define whether the window can get resized:
    glfwWindowHint( GLFW_RESIZABLE, true );

    /////////////////////////////////////////////////////////////////////////////////////
    // try to create an OpenGL context in a window and check the supported OpenGL version:
    //                                                  R,G,B,A, Depth,Stencil
    g_window = glfwCreateWindow( windowWidth, windowHeight, "Basic Techniques in Computer Graphics", NULL, NULL);
    if (!g_window) {
        cerr << "Failed to open a GLFW window" << endl;
        return false;
    }
    glfwMakeContextCurrent(g_window);

    return true;
}

void initializeOpenGL() 
{
	int glInit = ogl_LoadFunctionsForDebug( GL_TRUE, GL_TRUE );
	
	if (glInit != ogl_LOAD_SUCCEEDED) {
		cerr << "could not initialize OpenGL" << endl;
		exit(1);
	}
	
	if (ogl_ext_KHR_debug) {
		glDebugMessageCallback( debugCallback, NULL );
	}
    
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// some helpers:
//
// reads a textfile and returns the content as one string:
std::string getFileContent(const std::string &fileName) {
    std::string line = "";
    std::string fileContent = "";

    std::ifstream fileStream(fileName.c_str(), std::ifstream::in);

    if(fileStream.is_open()) {
        while(fileStream.good()) {
            std::getline(fileStream, line);
            fileContent += line + "\n";
        }
        fileStream.close();
    } else {
        std::cout << "Failed to open file: " << fileName << std::endl;
        return "";
    }
    return fileContent;
}

// check for shader compile errors:
void printGLSLCompileLog(GLuint shaderHandle) {
    GLint shaderError;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &shaderError);
    if(shaderError != GL_TRUE) {
        // yes, errors
        std::cerr << "Shader compile error: " << std::endl;
    }

    // a log gets always printed (could be warnings)
    GLsizei length = 0;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
    if(length > 1) {
        // a compile log can get produced even if there were no errors, but warnings!
        GLchar* pInfo = new char[length + 1];
        glGetShaderInfoLog(shaderHandle, length, &length, pInfo);
        std::cerr << "Compile log: " << std::string(pInfo) << std::endl;
        delete[] pInfo;
    }
}

void printGLSLLinkLog(GLuint progHandle) {
    // check for program link errors:
    GLint programError;
    glGetProgramiv(progHandle, GL_LINK_STATUS, &programError);

    if(programError != GL_TRUE) {
        // yes, errors :-(
        std::cerr << "Program could not get linked:" << std::endl;
    }

    GLsizei length = 0;
    glGetProgramiv(progHandle, GL_INFO_LOG_LENGTH, &length);
    if(length > 1) {
        // error log or warnings:
        GLchar* pInfo = new char[length + 1];
        glGetProgramInfoLog(progHandle, length, &length, pInfo);
        std::cout << "Linker log: " << std::string(pInfo) << std::endl;
        delete[] pInfo;
    }
}

glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far) {

    float phiHalfInRadians = 0.5f * phiInDegree * (M_PI / 180.0f);
    float t = near * tan(phiHalfInRadians);
    float b = -t;
    float left = b * aspectRatio;
    float right = t * aspectRatio;

    return glm::frustum(left, right, b, t, near, far);
}

void resizeCallback(int newWidth, int newHeight) {
    // define the part of the screen OpenGL should draw to (in pixels):
    glViewport(0, 0, newWidth, newHeight);
    g_ProjectionMatrix = buildFrustum(70.0f, (newWidth / (float) newHeight), 0.1f, 100.0f);
}

void setupShaderUniforms(GLuint _program, int _scene) {

    GLint projectionMatrixLocation = -1;
    GLint modelViewMatrixLocation = -1;
    GLint lightPosition = -1;
    GLint lightColor = -1;
    GLint lightSpotLightFactor = -1;
    GLint ambientMaterial = -1;
    GLint diffuseMaterial = -1;
    GLint specularMaterial = -1;
    GLint specularityExponent = -1;

    // Get uniform locations
    projectionMatrixLocation = glGetUniformLocation(_program, "uProjectionMatrix");
    modelViewMatrixLocation = glGetUniformLocation(_program, "uModelViewMatrix");
    lightPosition = glGetUniformLocation(_program, "uLightPosition");
    lightColor = glGetUniformLocation(_program, "uLightColor");
    lightSpotLightFactor = glGetUniformLocation(_program, "uLightSpotLightFactor");
    ambientMaterial = glGetUniformLocation(_program, "uAmbientMaterial");
    diffuseMaterial = glGetUniformLocation(_program, "uDiffuseMaterial");
    specularMaterial = glGetUniformLocation(_program, "uSpecularMaterial");
    specularityExponent = glGetUniformLocation(_program, "uSpecularityExponent");

    // Set uniform data
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ProjectionMatrix));
    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_ModelViewMatrix));
    if(_scene != 3) {
        glUniform3fv(lightPosition, 1, glm::value_ptr(vLightPosition1));
        glUniform3fv(lightColor, 1, glm::value_ptr(vLightColor1));
        glUniform1f(lightSpotLightFactor, fLightSpotLightFactor1);
    } else {
        glUniform3fv(lightPosition, 1, glm::value_ptr(vLightPosition2));
        glUniform3fv(lightColor, 1, glm::value_ptr(vLightColor2));
        glUniform1f(lightSpotLightFactor, fLightSpotLightFactor2);
    }
    glUniformMatrix3fv(ambientMaterial, 1, GL_FALSE, glm::value_ptr(mAmbientMaterial));
    glUniformMatrix3fv(diffuseMaterial, 1, GL_FALSE, glm::value_ptr(mDiffuseMaterial));
    glUniformMatrix3fv(specularMaterial, 1, GL_FALSE, glm::value_ptr(mSpecularMaterial));
    glUniform1f(specularityExponent, mSpecularityExponent);
}

void createShaderProgram(GLuint &vs, GLuint &fs, GLuint &prog, const std::string &vsFileName,
                         const std::string &fsFileName) {

    // create shader & shader program:
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    prog = glCreateProgram();

    // compile sources and add to shader program:
    std::string vsrc = getFileContent(vsFileName);
    const char *vsrc_c = vsrc.c_str();
    glShaderSource(vs, 1, &vsrc_c, NULL);
    glCompileShader(vs);
    printGLSLCompileLog(vs);
    glAttachShader(prog, vs);

    std::string fsrc = getFileContent(fsFileName);
    const char *fsrc_c = fsrc.c_str();
    glShaderSource(fs, 1, &fsrc_c, NULL);
    glCompileShader(fs);
    printGLSLCompileLog(fs);
    glAttachShader(prog, fs);

    // Bind attrib locations:
    // NOTE: trying to bind locations that are not present in the shader might result in
    // warnings but should not be problematic, so we just try to bind all attributes here
    // independent on the actual shader!
    glBindAttribLocation(prog, 0, "aPosition");
    glBindAttribLocation(prog, 1, "aNormal");

    // link shader program (remember to do this _after_ binding attrib. locations!):
    glLinkProgram(prog);
    printGLSLLinkLog(prog);
}

bool prepareExercise() {
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create VBO:
    glGenBuffers(1, &teapot_vbo);

    // upload data to graphic card:
    glBindBuffer(GL_ARRAY_BUFFER, teapot_vbo);
    glBufferData(GL_ARRAY_BUFFER, g_numberOfTeapotVertices * g_teapotVertexStrideSize, (void*) g_teapotVertices, GL_STATIC_DRAW);

    // create element array for the elements
    glGenBuffers(1, &teapot_ea);

    // upload data to graphic card:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapot_ea);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_numberOfTeapotTriangles * g_teapotElementStrideSize, (void*) g_teapotElements, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);

     // Vertex positions in attrib 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, g_teapotVertexStrideSize, 0);
    glEnableVertexAttribArray(0);

    // Vertex normals in attrib 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, g_teapotVertexStrideSize, (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    return true;
}

void cleanupExercise() {

    glDeleteBuffers(1, &teapot_vbo);
    glDeleteBuffers(1, &teapot_ea);

    glDeleteVertexArrays(1, &vao);
}

void callStudentCode( int sceneToDraw, double runTime )
{
	drawScene( sceneToDraw, runTime );
}

void resizeCallback( GLFWwindow* p, int newWidth, int newHeight )
{
    windowWidth  = newWidth;
    windowHeight = newHeight;
    
    resizeCallback( windowWidth, windowHeight );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////




int main( int argc, char *argv[] )
{
	printStudents();
	cout << endl;
	
    // Initialise GLFW
    if ( !glfwInit() )
    {
        cerr << "Failed to initialize GLFW" << endl;
        exit( -1 );
    }
    
    // get a 3.2 context:
    bool windowOK = createWindow( true );
	
	if ( !windowOK ) {
		cerr << "failed to create window" << endl;
		glfwTerminate();
		exit( -1 );
	}
	initializeOpenGL();
    
    glfwSetWindowTitle( g_window, "ACG - Introduction to CG - assignment" );
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode( g_window, GLFW_STICKY_KEYS, 1 );
	glfwSetWindowSizeCallback(  g_window, resizeCallback );

    // Enable vertical sync (on cards that support it)
    // vertical sync
    int vSync = 1;
    glfwSwapInterval( vSync );

    bool exitProgram = false;
    bool holdDownAKey = false;
    bool holdDownBKey = false;
    bool holdDownCKey = false;
    bool holdDownDKey = false;
	bool holdDownVKey = false;
    int sceneToDraw = 1;
    glGetError(); // clear errors
	
	if(!prepareExercise())
        exit(-1);
	
	initCustomResources();
	resizeCallback( windowWidth, windowHeight );
	
    double startTimeInSeconds = glfwGetTime();
    do {
	    glfwPollEvents();
		
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        double runTime = glfwGetTime() - startTimeInSeconds;
		
        callStudentCode( sceneToDraw, runTime );

        // Swap buffers
        glfwSwapBuffers( g_window );
        
        if ((glfwGetKey(g_window, 'A') == GLFW_PRESS) && (!holdDownAKey)) {
            holdDownAKey = true;
            sceneToDraw  = 1;
        }
        if ((glfwGetKey(g_window, 'A') == GLFW_RELEASE) && (holdDownAKey)) holdDownAKey = false;
        
        if ((glfwGetKey(g_window, 'B') == GLFW_PRESS) && (!holdDownBKey)) {
            holdDownBKey = true;
            sceneToDraw  = 2;
        }
        if ((glfwGetKey(g_window, 'B') == GLFW_RELEASE) && (holdDownBKey)) holdDownBKey = false;
        
        if ((glfwGetKey(g_window, 'C') == GLFW_PRESS) && (!holdDownCKey)) {
            holdDownCKey = true;
            sceneToDraw  = 3;
        }
        if ((glfwGetKey(g_window, 'C') == GLFW_RELEASE) && (holdDownCKey)) holdDownCKey = false;

        if ((glfwGetKey(g_window, 'D') == GLFW_PRESS) && (!holdDownDKey)) {
            holdDownDKey = true;
            sceneToDraw  = 4;
        }
        if ((glfwGetKey(g_window, 'D') == GLFW_RELEASE) && (holdDownDKey)) holdDownDKey = false;
        
		
		if ((glfwGetKey(g_window, 'V') == GLFW_PRESS) && (!holdDownVKey)) {
        	holdDownVKey = true;
        	vSync = (vSync+1)%2;
        	cout << "vsync is ";
        	if (vSync) {cout << "on";} else {cout << "off";}
        	cout << endl;
        	glfwSwapInterval(vSync);
        }
        if((glfwGetKey(g_window, 'V') == GLFW_RELEASE) && (holdDownVKey))
            holdDownVKey = false;
		
        
        if (glfwGetKey(g_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) exitProgram = true;
        
    } // Check if the window was closed
    while( !glfwWindowShouldClose( g_window ) && !exitProgram );
    
    // clean up:
    deleteCustomResources();
	cleanupExercise();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    exit(0);
}
