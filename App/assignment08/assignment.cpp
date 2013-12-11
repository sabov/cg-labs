/*
 * Basics of Computer Graphics Exercise
 */

#include "Tools/ABReader.hpp"
#include "Tools/PNGReader.hpp"
#include "Tools/TextureData.hpp"
#include "Tools/HelperFunctions.hpp"

#include "assignment.h"
using namespace std;

extern glm::mat4 g_ProjectionMatrix;

VertexArrayObject* g_vaoKilleroo;
ArrayBuffer*       g_abKilleroo;

GLuint textureA;
GLuint textureB;

ShaderProgram* shaderA; // you might need more than one program

// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Name 0, matrikel number 0" << endl;
    cout << "Student Name 1, matrikel number 1" << endl;
    cout << "Student Name 2, matrikel number 2" << endl;
    cout << "Student Name 3, matrikel number 3" << endl;
}

void drawScene(int scene, float runTime) {

    // Create model-view matrix
    glm::mat4 viewMatrix   = g_camera.getViewMatrix();
    glm::mat4 modelMatrix  = glm::scale( glm::vec3(1.5f) );
	
	if (scene == 1) {
		// ignore the color space and use a RGB texture for the diffuse and ambient material
		// the specular material should be white (set as a constant in the fragment shader:
		glm::mat4 modelViewMatrix    = viewMatrix * modelMatrix;
    	glm::mat4 invTranspModelView = glm::inverse(glm::transpose(modelViewMatrix));

		shaderA->use();
		shaderA->setUniform("uProjectionMatrix",          g_ProjectionMatrix );
    	shaderA->setUniform("uModelViewMatrix",           modelViewMatrix);
    	shaderA->setUniform("uInvTranspModelViewMatrix",  invTranspModelView);
		shaderA->setUniform("uLightPosition",             glm::vec3( modelViewMatrix*vLightPosition) );
        shaderA->setUniform("uTexKiller", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureA);
    
		// todo: texturing
		g_vaoKilleroo->render();
		
	} else if (scene == 2) {
		
		// use a sRGB texture for the diffuse and ambient material.
		// make sure the framebuffer is set to convert the linear content to sRGB!
		// the specular material should be white (set as a constant in the fragment shader:
	glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureB);
	glEnable(GL_FRAMEBUFFER_SRGB);
	g_vaoKilleroo->render();
	  
		
	} else if (scene == 3) {
		// use a sRGB texture for the diffuse and ambient material.
		// make sure the framebuffer is set to convert the linear content to sRGB!
		// the specular material should be read from a texture but should be gray scale (remember that this was defined in a linear space!)
		
	}
}


void initCustomResources() {

    // Create your resources here, e.g. shaders, buffers,...

    ////////////////////////////////////////////////////////////////////////////
    // Shader:
    
    shaderA = new ShaderProgram("partA_123456.vsh", "partA_123456.fsh");
    if (!shaderA->link()) exit(0);

    // Set uniforms that don't change:
    shaderA->use();
    shaderA->setUniform( "uLightColor",             vLightColor);
    shaderA->setUniform( "uSpecularityExponent",    fSpecularityExponent);

    ////////////////////////////////////////////////////////////////////////////
    // Define geometry:

    ABReader abreader;
	g_abKilleroo  = abreader.readABFile("killeroo.ab");
    g_vaoKilleroo = new VertexArrayObject();
    g_vaoKilleroo->attachAllMatchingAttributes(g_abKilleroo, shaderA);

    ////////////////////////////////////////////////////////////////////////////
    // Read textures:
    PNGReader* reader = new PNGReader();;
    TextureData* img = reader->readPNGFile("killerooDiffuse.png");


     glActiveTexture( GL_TEXTURE0 );
    glGenTextures(1, &textureA);
    glBindTexture( GL_TEXTURE_2D, textureA);
    glTexImage2D( GL_TEXTURE_2D, 0, img->getFormat(), img->getWidth(), img->getHeight(), 0,
    img->getFormat(), img->getType(), img->getData());
    glGenerateMipmap( GL_TEXTURE_2D );

    glActiveTexture( GL_TEXTURE0 );
    glGenTextures(1, &textureB);
    glBindTexture( GL_TEXTURE_2D, textureB);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB8 , img->getWidth(), img->getHeight(), 0,
    img->getFormat(), img->getType(), img->getData());
    glGenerateMipmap( GL_TEXTURE_2D );

    glEnable(GL_DEPTH_TEST);
}

void deleteCustomResources() {

    // Don't forget to delete your OpenGL resources (shaders, buffers, etc.)!

    delete shaderA;
    delete g_vaoKilleroo;
    delete g_abKilleroo;   
}

