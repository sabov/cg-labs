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
GenericCamera g_camera;

unsigned int windowWidth  = 512;
unsigned int windowHeight = 512;
float nearPlane =   0.1f;
float farPlane  = 100.0f;
GLFWwindow* g_window;

// assigment specific includes:

// assignment specific variables:
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
	
	// the framebuffer should support sRGB if possible - note: this does not activate the support itself!
	glfwWindowHint( GLFW_SRGB_CAPABLE, true );

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

bool prepareExercise() {
    return true;
}

void cleanupExercise() {

}

void callStudentCode( int sceneToDraw, double runTime )
{
	drawScene( sceneToDraw, runTime );
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
    g_ProjectionMatrix = buildFrustum(50.0f, (newWidth / (float) newHeight), 0.1f, 100.0f);
}

void resizeCallback( GLFWwindow* p, int newWidth, int newHeight )
{
    windowWidth  = newWidth;
    windowHeight = newHeight;
    
    resizeCallback( windowWidth, windowHeight );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void handleInput( double time )
{
    static double timeOfLastFrame = 0.0;
    if (timeOfLastFrame == 0.0) {
        timeOfLastFrame = time;
        // ignore the first frame, as the user has not done useful inputs till now anyway
        return;
    }

    // make camera movements based on the elapsed time and not based on frames rendered!
    double timeElapsed = time - timeOfLastFrame;

    double speed = 5.0; // magic value to scale the camera speed

    // as long as the keys are hold down, these are triggered each frame:
    if (glfwGetKey( g_window, 'W')) { // upper case!
        g_camera.moveForward( timeElapsed*speed );
    }
    if (glfwGetKey( g_window, 'A')) { // upper case!
        g_camera.moveLeft( timeElapsed*speed );
    }
    if (glfwGetKey( g_window, 'S')) { // upper case!
        g_camera.moveBack( timeElapsed*speed );
    }
    if (glfwGetKey( g_window, 'D')) { // upper case!
        g_camera.moveRight( timeElapsed*speed );
    }


    timeOfLastFrame = time;
}

void mouseMoveCallback( GLFWwindow *, double x, double y )
{
    static glm::vec2 initialPosition;
    static bool leftMouseButtonDown = false;
    
    if (!leftMouseButtonDown && glfwGetMouseButton( g_window, GLFW_MOUSE_BUTTON_1 )) {
        leftMouseButtonDown = true;
        initialPosition = glm::vec2( x,y );
        glfwSetInputMode( g_window, GLFW_CURSOR_HIDDEN, true ); // hide the cursor
        return;
    } else if (leftMouseButtonDown && !glfwGetMouseButton( g_window, GLFW_MOUSE_BUTTON_1 )) {
        leftMouseButtonDown = false;
        glfwSetInputMode( g_window, GLFW_CURSOR_HIDDEN, false ); // unhide the cursor
        glfwSetCursorPos( g_window, initialPosition.x, initialPosition.y );
        return;
    }

    glm::vec2 movement = glm::vec2( x,y ) - initialPosition;

    if (leftMouseButtonDown) {
        glm::vec2 realtiveMovement = glm::vec2(movement) / glm::vec2(windowWidth, windowHeight);
        g_camera.FPSstyleLookAround( realtiveMovement.x, realtiveMovement.y );
        //glfwSetCursorPos( g_window, initialPosition.x, initialPosition.y );
        initialPosition = glm::vec2( x,y );
    }
}


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
	// handle the mouse as a callback:
    glfwSetCursorPosCallback( g_window, mouseMoveCallback );
	
	g_camera.setPosition( glm::vec3(-3.498789, 6.385866, -8.150393) );
	g_camera.setTarget(   glm::vec3(-3.446630, 6.353455, -8.071470) );
	
	
    double startTimeInSeconds = glfwGetTime();
    do {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        double runTime = glfwGetTime() - startTimeInSeconds;
		
		glfwPollEvents();
		handleInput( runTime );

        if ((glfwGetKey(g_window, '1') == GLFW_PRESS) && (!holdDownAKey)) {
            holdDownAKey = true;
            sceneToDraw  = 1;
        }
        if ((glfwGetKey(g_window, '1') == GLFW_RELEASE) && (holdDownAKey)) holdDownAKey = false;
        
        if ((glfwGetKey(g_window, '2') == GLFW_PRESS) && (!holdDownBKey)) {
            holdDownBKey = true;
            sceneToDraw  = 2;
        }
        if ((glfwGetKey(g_window, '2') == GLFW_RELEASE) && (holdDownBKey)) holdDownBKey = false;
        
        if ((glfwGetKey(g_window, '3') == GLFW_PRESS) && (!holdDownCKey)) {
            holdDownCKey = true;
            sceneToDraw  = 3;
        }
        if ((glfwGetKey(g_window, '3') == GLFW_RELEASE) && (holdDownCKey)) holdDownCKey = false;

        if ((glfwGetKey(g_window, '4') == GLFW_PRESS) && (!holdDownDKey)) {
            holdDownDKey = true;
            sceneToDraw  = 4;
        }
        if ((glfwGetKey(g_window, '4') == GLFW_RELEASE) && (holdDownDKey)) holdDownDKey = false;
        
		
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
		
		
		
		callStudentCode( sceneToDraw, runTime );
        // Swap buffers
        glfwSwapBuffers( g_window );
        
    } // Check if the window was closed
    while( !glfwWindowShouldClose( g_window ) && !exitProgram );
    
    // clean up:
    deleteCustomResources();
	cleanupExercise();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    exit(0);
}
