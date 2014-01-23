/*
 * Basics of Computer Graphics Exercise: Splat Rendering
 */

#include "Tools/ABReader.hpp"
#include "Tools/PBReader.hpp"
#include "Tools/TextureData.hpp"
#include "Tools/ShaderProgram.hpp"
#include "Tools/HelperFunctions.hpp"

#include "assignment.h"
using namespace std;

/*
 * Some variables we'll need
 */
extern const float g_phi;
extern int g_windowHeight;
extern int g_windowWidth;
extern const float g_near;
extern const float g_far;

extern glm::mat4 g_modelViewMatrix;
extern glm::mat4 g_projectionMatrix;

ShaderProgram* g_simplePointShader;
ShaderProgram* g_ssSplatShader;
ShaderProgram* g_osSplatShader;
ShaderProgram* g_osBlendSplatShader;

VertexArrayObject* g_vaoChameleon;
ArrayBuffer* g_abChameleon;

// Add your team member's names and matriculation numbers here:
void printStudents() {
    cout << "Student Name 0, matriculation number 0" << endl;
    cout << "Student Name 1, matriculation number 1" << endl;
    cout << "Student Name 2, matriculation number 2" << endl;
    cout << "Student Name 3, matriculation number 3" << endl;
}

void drawScene(int scene) {

    // =======================================================================
    // =======================================================================
    // Assignment code:
    // Part B: ( required for B-D )
    // =======================================================================
    // =======================================================================


    // Replace with your code

    float scaleToScreen = -g_windowHeight / tan(g_phi * M_PI / 360);

    // =======================================================================
    // End assignment code
    // =======================================================================
    
    if(scene == 1) {

        g_simplePointShader->use();
        g_simplePointShader->setUniform("uModelViewMatrix", g_modelViewMatrix);
        g_simplePointShader->setUniform("uProjectionMatrix", g_projectionMatrix);

        g_vaoChameleon->render();

    } else if(scene == 2) {

        g_ssSplatShader->use();
        g_ssSplatShader->setUniform("uModelViewMatrix", g_modelViewMatrix);
        g_ssSplatShader->setUniform("uProjectionMatrix", g_projectionMatrix);
        g_ssSplatShader->setUniform("uScaleToScreen", scaleToScreen);

        g_vaoChameleon->render();

    } else if(scene == 3) {

        g_osSplatShader->use();
        g_osSplatShader->setUniform("uModelViewMatrix", g_modelViewMatrix);
        g_osSplatShader->setUniform("uInvTranspModelViewMatrix", glm::inverse(glm::transpose(g_modelViewMatrix))); // for the normals
        g_osSplatShader->setUniform("uProjectionMatrix", g_projectionMatrix);
        g_osSplatShader->setUniform("uInverseProjectionMatrix", glm::inverse(g_projectionMatrix) );
        g_osSplatShader->setUniform("uWindowWidth", (float)g_windowWidth);
        g_osSplatShader->setUniform("uWindowHeight", (float)g_windowHeight);
        g_osSplatShader->setUniform("uNear", g_near);
        g_osSplatShader->setUniform("uFar", g_far);
        g_osSplatShader->setUniform("uScaleToScreen", scaleToScreen);

        g_vaoChameleon->render();

    } else if(scene == 4) {

        g_osBlendSplatShader->use();
        g_osBlendSplatShader->setUniform("uModelViewMatrix", g_modelViewMatrix);
        g_osBlendSplatShader->setUniform("uInvTranspModelViewMatrix", glm::inverse(glm::transpose(g_modelViewMatrix)));
        g_osBlendSplatShader->setUniform("uProjectionMatrix", g_projectionMatrix);
        g_osBlendSplatShader->setUniform("uInverseProjectionMatrix", glm::inverse(g_projectionMatrix) );
        g_osBlendSplatShader->setUniform("uWindowWidth", (float)g_windowWidth);
        g_osBlendSplatShader->setUniform("uWindowHeight", (float)g_windowHeight);
        g_osBlendSplatShader->setUniform("uNear", g_near);
        g_osBlendSplatShader->setUniform("uFar", g_far);
        g_osBlendSplatShader->setUniform("uScaleToScreen", scaleToScreen);

        // Pass the render pass to the shaders
        g_osBlendSplatShader->setUniform("uRenderPass", (int)0);

        // =======================================================================
        // =======================================================================
        // Assignment code:
        // Part D:
        // =======================================================================
        // =======================================================================

        // Replace with your code
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        g_vaoChameleon->render();
        g_osBlendSplatShader->setUniform("uRenderPass", (int)1);

        glDepthMask(GL_FALSE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        g_vaoChameleon->render();

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        // =======================================================================
        // End assignment code
        // =======================================================================
    }
}

void initCustomResources() {

    // Initialize shaders:
    g_simplePointShader = new ShaderProgram("scene1_123456.vsh", "scene1_123456.fsh");
    g_simplePointShader->bindAttributeLocation("aPosition",  0);
    g_simplePointShader->bindAttributeLocation("aColor",     2);
    if(!g_simplePointShader->link())
        exit(0);

    g_ssSplatShader = new ShaderProgram("scene2_123456.vsh", "scene2_123456.fsh");
    g_ssSplatShader->bindAttributeLocation("aPosition",  0);
    g_ssSplatShader->bindAttributeLocation("aColor",     2);
    g_ssSplatShader->bindAttributeLocation("aSplatSize", 3);
    if(!g_ssSplatShader->link())
        exit(0);

    g_osSplatShader = new ShaderProgram("scene3_123456.vsh", "scene3_123456.fsh");
    g_osSplatShader->bindAttributeLocation("aPosition",  0);
    g_osSplatShader->bindAttributeLocation("aNormal",    1);
    g_osSplatShader->bindAttributeLocation("aColor",     2);
    g_osSplatShader->bindAttributeLocation("aSplatSize", 3);
    if(!g_osSplatShader->link())
        exit(0);

    g_osBlendSplatShader = new ShaderProgram("scene4_123456.vsh", "scene4_123456.fsh");
    g_osBlendSplatShader->bindAttributeLocation("aPosition",  0);
    g_osBlendSplatShader->bindAttributeLocation("aNormal",    1);
    g_osBlendSplatShader->bindAttributeLocation("aColor",     2);
    g_osBlendSplatShader->bindAttributeLocation("aSplatSize", 3);
    if(!g_osBlendSplatShader->link())
        exit(0);

    // Define geometry:
    PBReader pbreader;
    g_abChameleon = pbreader.readPBFile("chameleon.pb");

    // Define VAOs:
    g_vaoChameleon = new VertexArrayObject();
    g_vaoChameleon->setMode(GL_POINTS);
    g_vaoChameleon->attachAllMatchingAttributes(g_abChameleon, g_osBlendSplatShader);

    // Enable z-test
    glEnable(GL_DEPTH_TEST);

    // Allow to adjust projected vertex size in v-shader
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void deleteCustomResources() {

    delete g_vaoChameleon;
    delete g_abChameleon;

    delete g_simplePointShader;
    delete g_ssSplatShader;
    delete g_osSplatShader;
    delete g_osBlendSplatShader;
}

