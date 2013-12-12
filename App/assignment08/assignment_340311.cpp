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

GLuint killerooDiffuseTex;
GLuint killerooSpecularTex;
GLuint textureB;

ShaderProgram* shaderA; // you might need more than one program
ShaderProgram* shaderB; // you might need more than one program
ShaderProgram* shaderC; // you might need more than one program

// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Tarasenko Petro, matrikel number 340826" << endl;
    cout << "Student Oleksandr Sabov, matrikel number 340311" << endl;
    cout << "Student Chih-Yun Tsai, matrikel number 328782" << endl;
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
        glBindTexture(GL_TEXTURE_2D, killerooDiffuseTex);
        glDisable(GL_FRAMEBUFFER_SRGB);
    
        // todo: texturing
        g_vaoKilleroo->render();
        
    } else if (scene == 2) {
        
        // use a sRGB texture for the diffuse and ambient material.
        // make sure the framebuffer is set to convert the linear content to sRGB!
        // the specular material should be white (set as a constant in the fragment shader:
        glm::mat4 modelViewMatrix    = viewMatrix * modelMatrix;
        glm::mat4 invTranspModelView = glm::inverse(glm::transpose(modelViewMatrix));

        shaderB->use();
        shaderB->setUniform("uProjectionMatrix",          g_ProjectionMatrix );
        shaderB->setUniform("uModelViewMatrix",           modelViewMatrix);
        shaderB->setUniform("uInvTranspModelViewMatrix",  invTranspModelView);
        shaderB->setUniform("uLightPosition",             glm::vec3( modelViewMatrix*vLightPosition) );
        shaderB->setUniform("uTexKiller", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureB);
        glEnable(GL_FRAMEBUFFER_SRGB);

        g_vaoKilleroo->render();
      
        
    } else if (scene == 3) {
        // use a sRGB texture for the diffuse and ambient material.
        // make sure the framebuffer is set to convert the linear content to sRGB!
        // the specular material should be read from a texture but should be gray scale (remember that this was defined in a linear space!)
        
        glm::mat4 modelViewMatrix    = viewMatrix * modelMatrix;
        glm::mat4 invTranspModelView = glm::inverse(glm::transpose(modelViewMatrix));

        shaderC->use();
        shaderC->setUniform("uProjectionMatrix",          g_ProjectionMatrix );
        shaderC->setUniform("uModelViewMatrix",           modelViewMatrix);
        shaderC->setUniform("uInvTranspModelViewMatrix",  invTranspModelView);
        shaderC->setUniform("uLightPosition",             glm::vec3( modelViewMatrix*vLightPosition) );
        shaderC->setUniform("uTexDiffuse", 0);
        shaderC->setUniform("uTexSpecular", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, textureB);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, killerooSpecularTex);

        g_vaoKilleroo->render();

    }
}


void initCustomResources() {

    // Create your resources here, e.g. shaders, buffers,...

    ////////////////////////////////////////////////////////////////////////////
    // Shader:
    
    shaderA = new ShaderProgram("partA_340311.vsh", "partA_340311.fsh");
    if (!shaderA->link()) exit(0);

    shaderB = new ShaderProgram("partA_340311.vsh", "partA_340311.fsh");
    if (!shaderA->link()) exit(0);

    shaderC = new ShaderProgram("partC_340311.vsh", "partC_340311.fsh");
    if (!shaderA->link()) exit(0);

    // Set uniforms that don't change:
    shaderA->use();
    shaderA->setUniform( "uLightColor",             vLightColor);
    shaderA->setUniform( "uSpecularityExponent",    fSpecularityExponent);

    shaderB->use();
    shaderB->setUniform( "uLightColor",             vLightColor);
    shaderB->setUniform( "uSpecularityExponent",    fSpecularityExponent);

    shaderC->use();
    shaderC->setUniform( "uLightColor",             vLightColor);
    shaderC->setUniform( "uSpecularityExponent",    fSpecularityExponent);
    ////////////////////////////////////////////////////////////////////////////
    // Define geometry:

    ABReader abreader;
    g_abKilleroo  = abreader.readABFile("killeroo.ab");
    g_vaoKilleroo = new VertexArrayObject();
    g_vaoKilleroo->attachAllMatchingAttributes(g_abKilleroo, shaderA);

    ////////////////////////////////////////////////////////////////////////////
    // Read textures:
    PNGReader* reader = new PNGReader();;
    TextureData* killerooDiffuseImg = reader->readPNGFile("killerooDiffuse.png");
    TextureData* killerooSpecularImg = reader->readPNGFile("killerooSpecular.png");


    //diffuse
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures(1, &killerooDiffuseTex);
    glBindTexture( GL_TEXTURE_2D, killerooDiffuseTex);
    glTexImage2D( GL_TEXTURE_2D, 0, killerooDiffuseImg->getFormat(), killerooDiffuseImg->getWidth(), killerooDiffuseImg->getHeight(), 0,
    killerooDiffuseImg->getFormat(), killerooDiffuseImg->getType(), killerooDiffuseImg->getData());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap( GL_TEXTURE_2D );

    //specular
    glActiveTexture( GL_TEXTURE1 );
    glGenTextures(1, &killerooSpecularTex);
    glBindTexture( GL_TEXTURE_2D, killerooSpecularTex);
    glTexImage2D( GL_TEXTURE_2D, 0, killerooSpecularImg->getFormat(), killerooSpecularImg->getWidth(), killerooSpecularImg->getHeight(), 0,
    killerooSpecularImg->getFormat(), killerooSpecularImg->getType(), killerooSpecularImg->getData());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap( GL_TEXTURE_2D );

    //partB
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures(1, &textureB);
    glBindTexture( GL_TEXTURE_2D, textureB);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB8 , killerooDiffuseImg->getWidth(), killerooDiffuseImg->getHeight(), 0,
    killerooDiffuseImg->getFormat(), killerooDiffuseImg->getType(), killerooDiffuseImg->getData());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap( GL_TEXTURE_2D );

    glEnable(GL_DEPTH_TEST);
}

void deleteCustomResources() {

    // Don't forget to delete your OpenGL resources (shaders, buffers, etc.)!

    delete shaderA;
    delete g_vaoKilleroo;
    delete g_abKilleroo;   
}

