/*
 * Basics of Computer Graphics Exercise
 */

#include "assignment.h"
using namespace std;

extern glm::mat4 g_ProjectionMatrix;

// add your team members names and matrikel numbers here:
void printStudents() {
    cout << "Student Name 0, matrikel number 0" << endl;
    cout << "Student Name 1, matrikel number 1" << endl;
    cout << "Student Name 2, matrikel number 2" << endl;
    cout << "Student Name 3, matrikel number 3" << endl;
}

//16 queries
GLuint query[16];
GLuint queryResults[16];
GLuint params[16];

// for part D
bool performQuery = true;
bool hasBeenTested = false;
int  framesWaited = 0;

void drawScene(int scene,double) {
    static int lastScene = 0;

    if (scene == 1) {
        for (int i = 0; i < 16; i++) {queryResults[i] = true;}
        for (int i = 0; i < 16; i++) {
                     // =======================================================================
                     // =======================================================================
                     // Assignment code:
                     // Part A:
                     //  one query per bunny, render the bounding box with a query, if that
                     //  would generate fragments draw the bunny
                     //
                     //  Use g_bunnyVisibility[i]=true/false to pass information to the
                     //  drawScenePreview function
                     // =======================================================================
                     // =======================================================================


            glBeginQuery(GL_SAMPLES_PASSED, query[i]);

                glColorMask(false, false, false, false);
                glDepthMask(false);
                g_bunnyVisibility[i] = false;
                drawBoundingObject(i);

            glEndQuery(GL_SAMPLES_PASSED);

            do {
                glGetQueryObjectuiv(query[i], GL_QUERY_RESULT_AVAILABLE, &queryResults[i]);
            } while (!queryResults[i]);

            glGetQueryObjectuiv(query[i], GL_QUERY_RESULT, &params[i]);

            //glBeginConditionalRender(query[i], GL_QUERY_WAIT);

                glColorMask(true, true, true, true);
                glDepthMask(true);
                if(params[i] > 0.0) {
                    g_bunnyVisibility[i] = true;
                    drawBunny(i);
                } else {
                    drawBoundingObject(i);
                }

            //glEndConditionalRender();


                     // =======================================================================
                     // End assignment code
                     // =======================================================================
        }
    } else if (scene == 2) {
                // =======================================================================
                // =======================================================================
                // Assignment code:
                // Part B:
                //
        // assume all bunnies to be visible in the first frame
        // start 16 occlusion queries, render the bounding objects
        // in case the bunny was not visible before and the bunny in case it was visible.
        // one frame later evaluate the results.
                //
                // =======================================================================
                // =======================================================================

        bool firstFrame = true;
        for (int i = 0; i < 16; i++) {
        
            glBeginQuery(GL_SAMPLES_PASSED, query[i]);

            glColorMask(false, false, false, false);
            glDepthMask(false);
            g_bunnyVisibility[i] = false;
            drawBoundingObject(i);

            glEndQuery(GL_SAMPLES_PASSED);

            if(firstFrame || queryResults[i]) {
                if(firstFrame) {
                    firstFrame = false;
                } else {
                    glGetQueryObjectuiv(query[i], GL_QUERY_RESULT, &params[i]);
                }
                glGetQueryObjectuiv(query[i], GL_QUERY_RESULT_AVAILABLE, &queryResults[i]);
            }


            glColorMask(true, true, true, true);
            glDepthMask(true);
            if(params[i] > 0.0) {
                g_bunnyVisibility[i] = true;
                drawBunny(i);
            } else {
                drawBoundingObject(i);
            }


                     // =======================================================================
                     // End assignment code
                     // =======================================================================
        }
 
                // =======================================================================
                // End assignment code
                // =======================================================================
            
    } else if (scene == 3) {
        
        for (int i = 0; i < 16; i++) {

                  // =======================================================================
                  // =======================================================================
                  // Assignment code:
                  // Part C:
                  //
                  // generate a query and render the bounding boxes, depending on the result 
                  // render the bunny.
                  // let the GPU decide and never actually query the result.
                  // never reading the result back from the GPU helps to avoid stalls.
                  //
                  // =======================================================================
                  // =======================================================================
        

                  // =======================================================================
                  // End assignment code
                  // =======================================================================

        }
    } else {
        // =======================================================================
                // =======================================================================
                // Assignment code:
                // Part D:
        // 
        // render all bunnies each frame starting a query at the first frame and check if it is
        // available each frame. if it is available, print the frames that have passed and
        // start a new query in the next frame.
        // 
        // if a result would be available directly, the output would be 0,
        // if it is available after one frame, it would be 1 etc.
        // actual wait times are dependent on the GPU, OS and whether vsync is active.
                // =======================================================================
                // =======================================================================



                // =======================================================================
                // End assignment code
                // =======================================================================	        
    }
    
    lastScene = scene;
}

//
// draw a preview on the right of the window
// we can use the g_bunnyVisibility array to get visibility information from 
// drawScene to here.
// for part B we can use also conditional rendering with the same queries as in drawScene
// for part D just draw all bunnies
//
void drawScenePreview(int scene,double) {
    if (scene <= 2) {
        for (int i = 0; i < 16; i++) {
                       // =======================================================================
                       // =======================================================================
                       // Assignment code:
                       // Part A/B:
                       //
                       //  Use g_bunnyVisibility[i] an render the boundingbox or the bunny
                       // =======================================================================
                       // =======================================================================


            if(g_bunnyVisibility[i]) {
                drawBunny(i);
            } else {
                drawBoundingObject(i);
            }



                        // =======================================================================
                        // End assignment code
                        // =======================================================================

        }
    } else if (scene == 3) {
        for (int i = 0; i < 16; i++) {
                   // =======================================================================
                   // =======================================================================
                   // Assignment code:
                   // Part C:
                   //
                   //  Remember to use conditional rendering for the bunny here as well!
                   //
                   // =======================================================================
                   // =======================================================================


           // =======================================================================
                   // End assignment code
                   // =======================================================================

        }
    } else {
        for (int i = 0; i < 16; i++) {
            drawBunny( i );
        }
    }
}

void initCustomResources() {
    // Create your resources here, e.g. shaders, buffers,...
    glGenQueries(16, query);
}

void deleteCustomResources() {
    // Don't forget to delete your OpenGL resources (shaders, buffers, etc.)!
    glDeleteQueries( 16, query );
}

