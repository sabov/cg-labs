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


// assigment specific includes:
#include "Tools/ABReader.hpp"
#include "Tools/PPMReader.hpp"
#include "Tools/TextureData.hpp"
#include "Tools/HelperFunctions.hpp"

// assignment specific variables:
glm::mat4 g_ModelViewMatrix;
glm::mat4 g_ProjectionMatrix;
glm::mat4 g_invTranspModelView;

ShaderProgram* g_shader;
VertexArrayObject* g_vaoBunny;
ArrayBuffer* g_abBunny;
ArrayBuffer* g_abCube;
VertexArrayObject* g_vaoCube;

GLuint bunnyTexture1;

bool g_bunnyVisibility[16];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// some variables we'll need:
//
unsigned int windowWidth  = 640;
unsigned int windowHeight = 480;
float nearPlane =   0.1f;
float farPlane  = 100.0f;
GLFWwindow* g_window;



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
    //glfwWindowHint( GLFW_RESIZABLE, true );

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
	
	
	cout << "OpenGL debug context:   ";
	if (g_ogl_native_debug_context) {
		cout << "native" << endl;
	} else {
		cout << "emulated" << endl;
	}
	
	if (ogl_ext_KHR_debug) {
		cout << "KHR_debug support:      present - ";
		if (g_ogl_native_KHR_debug) {
			cout << "native" << endl;
		} else {
			cout << "emulated" << endl;
		}
		glDebugMessageCallback( debugCallback, NULL );
	}
    
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far) {

    float phiHalfInRadians = 0.5f * phiInDegree * (M_PI / 180.0f);
    float t = near * tan(phiHalfInRadians);
    float b = -t;
    float left = b * aspectRatio;
    float right = t * aspectRatio;

    return glm::frustum(left, right, b, t, near, far);
}

void resizeCallback( GLFWwindow *, int newWidth, int newHeight )
{
    windowWidth  = newWidth;
    windowHeight = newHeight;

    // define the part of the screen OpenGL should draw to (in pixels):
    glViewport( 0, 0, newWidth, newHeight );
	
	int halfWidth = newWidth/2;
    // define the part of the screen OpenGL should draw to (in pixels):
    g_ProjectionMatrix = buildFrustum(75.0f, (halfWidth / (float) newHeight), 0.1f, 100.0f);
}


bool prepareExercise() {
	////////////////////////////////////////////////////////////////////////////
    // Shader:
    
    // you have to edit the shader for texture access, so rename them and read your shader
    // files here:
    g_shader = new ShaderProgram("phongSOLUTION.vsh", "phongSOLUTION.fsh");
    if (!g_shader->link()) exit(0);

    // Set uniforms that don't change:
    g_shader->use();
    g_shader->setUniform("uLightPosition",          vLightPosition);
    g_shader->setUniform("uLightColor",             vLightColor);
    g_shader->setUniform("uAmbientMaterial",        mAmbientMaterial);
    g_shader->setUniform("uSpecularMaterial",       mSpecularMaterial);
    g_shader->setUniform("uSpecularityExponent",    fSpecularityExponent);
    
    glActiveTexture( GL_TEXTURE0 );
    g_shader->setUniform( "uTexture", (int)0 );
    g_shader->setUniform( "uBlendColor", glm::vec3(1.0f,1.0f,1.0f ) );

    ////////////////////////////////////////////////////////////////////////////
    // Define geometry:

    ABReader abreader;
    g_abBunny    = abreader.readABFile("bunny.ab");
    g_abCube     = abreader.readABFile("cube.ab");

    // define VAOs:
    g_vaoBunny    = new VertexArrayObject();
    g_vaoBunny->attachAllMatchingAttributes(g_abBunny, g_shader);
    
    g_vaoCube = new VertexArrayObject();
    g_vaoCube->attachAllMatchingAttributes(g_abCube, g_shader);

    glEnable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////////////////////////////
    // Read textures:

    PPMReader ppmreader;
    TextureData* texture;
	
	texture = ppmreader.readPPMFile("clownfishBunny.ppm");

    glGenTextures(1, &bunnyTexture1);
    glBindTexture(GL_TEXTURE_2D, bunnyTexture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // MipMap level
        GL_RGBA,
        texture->getWidth(),
        texture->getHeight(),
        0,//no border !
        texture->getFormat(),
        texture->getType(),
        texture->getData() );

    glGenerateMipmap(GL_TEXTURE_2D);

    delete texture; // Not needed anymore!
    
    for (int i = 0; i < 16; i++) g_bunnyVisibility[i] = true;
    
	glEnable( GL_SCISSOR_TEST ); // the scissor test is like a primitive stencil test but is restricted to a rectangular region
	
	int halfWidth = windowWidth/2;
    // define the part of the screen OpenGL should draw to (in pixels):
    g_ProjectionMatrix = buildFrustum(75.0f, (halfWidth / (float) windowHeight), 0.1f, 100.0f);
	
    return true;
}

void cleanupExercise() {
	delete g_shader;
    delete g_vaoBunny;
    delete g_abBunny;
    delete g_vaoCube;
    delete g_abCube;

 	glDeleteTextures(1, &bunnyTexture1);
}

float runTime;
glm::mat4 viewMatrix;

glm::vec3 colors[] = { glm::vec3(1.0f,1.0f,1.0f),
	glm::vec3(0.0f,1.0f,1.0f),
	glm::vec3(1.0f,0.0f,1.0f),
	glm::vec3(1.0f,1.0f,0.0f),
	glm::vec3(0.0f,0.0f,1.0f),
	glm::vec3(0.0f,1.0f,0.0f),
	glm::vec3(1.0f,0.0f,0.0f),
	glm::vec3(0.5f,0.5f,0.5f)
};

void setUniformsForObject( int number ) {
	g_shader->setUniform( "uBlendColor", colors[number%8] );
    
    float posDir = 1.0;
    if (number < 8) posDir *= -1.0;
    float angle = number*45.0f + (posDir*runTime) * 360.0f / 20.0f;
	
	float offset = 7.5f;
	if (number < 8) offset = 0.0f;
	
	float scale = 0.333f;
	if (number < 8) scale = 0.5f;
	
    glm::mat4 modelMatrixBunny         = glm::rotate(angle, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::translate(offset+5.0f,0.0f,0.0f) * glm::scale(scale,scale,scale);
    
	glm::mat4 modelViewMatrix    = viewMatrix * modelMatrixBunny;
    glm::mat4 invTranspModelView = glm::inverse(glm::transpose(modelViewMatrix));
    g_shader->setUniform("uModelViewMatrix",            modelViewMatrix);
    g_shader->setUniform("uInvTranspModelViewMatrix",   invTranspModelView);
}

void drawBunny( int number ) {

	setUniformsForObject( number );
	g_shader->setUniform( "uBoundingObject", 0 );
	
	g_vaoBunny->render();
}

void drawBoundingObject( int number ) {
	setUniformsForObject( number );
	g_shader->setUniform( "uBoundingObject", 1 );
	g_vaoCube->render();
}

void callStudentCode( int sceneToDraw, double runTime )
{
	glEnable( GL_SCISSOR_TEST );
	glScissor(0, 0, windowWidth/2, windowHeight);  // to restrict the clear
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glScissor(windowWidth/2, 0, windowWidth/2, windowHeight);
	glClearColor(0.1,0.1,0.1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	// clear the full depthbuffer:
	glScissor(0, 0, windowWidth, windowHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable( GL_SCISSOR_TEST ); // only needed to work around a Mac driver bug

	glViewport(0, 0, windowWidth/2, windowHeight); // to restrict the rasterization
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0, 0.5, -1.0), glm::vec3(0, 1, 0));
	g_shader->setUniform("uProjectionMatrix", g_ProjectionMatrix );
	drawScene(sceneToDraw, runTime);

	glViewport(windowWidth/2, 0, windowWidth/2, windowHeight);
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, -1));
	float aspect = windowWidth / (windowHeight*2.0f);
	float size = 20.0f;
	g_shader->setUniform("uProjectionMatrix", glm::ortho(-aspect*size, aspect*size, -size, size ,0.1f,100.0f) );

	drawScenePreview( sceneToDraw, runTime );
	
	glDrawArrays( GL_POINTS, 0, 1 ); // draw one point (somewhere) as a work around of bug 12949224 on MacOS X
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
	
    double startTimeInSeconds = glfwGetTime();
    do {
	    glfwPollEvents();
		
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        runTime = glfwGetTime() - startTimeInSeconds;
		
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
