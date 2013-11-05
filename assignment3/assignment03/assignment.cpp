/*
 * Basics of Computer Graphics Exercise
 */
 
#include "assignment.h"
#include <gl_core_32.hh>
using namespace std;

// add your team members names and matrikel numbers here:
void printStudents()
{
	cout << "Student Name 0, matrikel number 0" << endl;
	cout << "Student Name 1, matrikel number 1" << endl;
	cout << "Student Name 2, matrikel number 2" << endl;
	cout << "Student Name 3, matrikel number 3" << endl;
}


glm::mat4 g_ViewMatrix;
glm::mat4 g_ProjectionMatrix;

/*
 * A blue circle represents the outer part of the track
 */
void drawTrack( const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection ) {

    drawCircle( glm::vec3(0.0, 0.0, 1.0), model, view, projection );
}

/*
 * A white circle represents the track center line
 */
void drawTrackMarks( const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection ) {
    drawCircle( glm::vec3(1.0, 1.0, 1.0), model, view, projection );
}

/*
 * A Blue circle represents the tracks outer boundary
 */
void drawOuterTrackMarks( const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection ) {
    drawCircle( glm::vec3(0.0, 0.0, 1.0), model, view, projection );
}

/*
 * A gray circle representing the grandstand
 */
void drawGrandstand( const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection ) {
    drawCircle( glm::vec3(0.4, 0.4, 0.4), model, view, projection );
}

glm::mat4 getRotationMatrixZAxis( float angle ) {
	glm::mat4 r;
	r[0][0] =  cos(angle);
	r[1][0] = -sin(angle);
	r[0][1] =  sin(angle);
	r[1][1] =  cos(angle);
    r[2][2] = 1.0;
    r[3][3] = 1.0;
	return r;
}

glm::mat4 getRotationMatrixYAxis( float angle ) {
    glm::mat4 r;
    r[0][0] =  cos(angle);
    r[2][0] =  sin(angle);
    r[1][1] = 1.0;
    r[0][2] =  -sin(angle);
    r[2][2] =  cos(angle);
    r[3][3] = 1.0;
    return r;
}

glm::mat4 buildFrustum( float phiInDegree, float aspectRatio, float near, float far) {

    glm::mat4 fm;

    // ====================================================================
    // buildFrustum function for programming exercise part b:
    // Add your code here:
    // ====================================================================

    // ====================================================================
    // End Exercise code
    // ====================================================================

    return fm;
}

glm::mat4 lookAt(const glm::vec3 &camPos, const glm::vec3 &viewDirection, const glm::vec3 &up) {

    // ====================================================================
    // Lookat for programming exercise part a:
    // Add your code here:
    // ====================================================================

    //return matrix;

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

void resizeCallback( int newWidth, int newHeight )
{
    glViewport( 0, 0, newWidth, newHeight );

    // ====================================================================
    // projection matrix setup for programming exercise part d:
    // Add your code here:
    // ====================================================================

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

void drawTrack() {
    
    glm::mat4 transformTrack;

    for(unsigned int i = 0; i < 30; ++i) {

        // Compute rotation for the marks
        float angle = float(i)*4.0f*M_PI/60.0f;

        float next_angle = float(i+1)*4.0f*M_PI/60.0f;

        float height = sin(angle*2.0) * 0.1;

        float next_height = sin(float(i+1)*4.0f*M_PI/60.0f * 2.0) * 0.1;

        glm::mat4 markRotation = getRotationMatrixZAxis( angle );

        // =====================================================
        // Track Center
        // =====================================================

        // Scaling
        glm::mat4 scaleTrack;
        scaleTrack[0][0] = 0.03;
        scaleTrack[1][1] = 0.01;
        scaleTrack[2][2] = 0.01;

        //translate
        glm::mat4 translateTrack;
        translateTrack[3][1] = 0.8; // Radius of the circle
        translateTrack[3][2] = height;  //Height

        // rotate around y axis to follow track height change
        glm::mat4 heightRotation = getRotationMatrixYAxis(-(M_PI/2.0 - acos( (height-next_height)/( 2.0 * sin((next_angle-angle)/2.0) )) ) );

        drawTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, g_ViewMatrix, g_ProjectionMatrix);

        // =====================================================
        // Track Boundaries
        // =====================================================

        // Outer Circle
        translateTrack[3][1] = 0.9; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, g_ViewMatrix, g_ProjectionMatrix);

        // Inner Circle
        translateTrack[3][1] = 0.7; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, g_ViewMatrix, g_ProjectionMatrix);
    }
}

void drawGrandstand() {

  glm::mat4 transformStand;

  // Scale circle to form grandstand
  transformStand[0][0] = 0.05;
  transformStand[1][1] = 0.6;
  transformStand[2][2] = 0.07;

  // Place on the left of the track
  transformStand[3][0] = -0.95;

  drawGrandstand(transformStand, g_ViewMatrix, g_ProjectionMatrix );
    
}

void drawFinish() {
    
  glm::mat4 transformFinish;

  // Scaling
  transformFinish[0][0] = 0.005;
  transformFinish[1][1] = 0.03;
  transformFinish[2][2] = 0.01;

  // Initialize translation (Left boundary of track)
  transformFinish[3][0] = -0.9;

  // Draw nine marks from left to right
  for ( unsigned int i = 0; i < 9 ; ++i) {

    // Translate to next mark
    transformFinish[3][0] += 0.02 ;
    
    drawTrackMarks(transformFinish, g_ViewMatrix, g_ProjectionMatrix);
}

}

void drawCar( float angle, int lane, glm::vec3 color )
{
    // Start with identity
    glm::mat4 transformCar;
    float height = sin(angle*2.0) * 0.1;

    // Scale second car to fit onto track
    transformCar[0][0] *= 0.04;
    transformCar[1][1] *= 0.1;
    transformCar[2][2] *= 0.03;
    transformCar[3][2] = -height;

    // Transform second car to position 0 (x-direction)
    if (lane == 1) {
        transformCar[3][0] = -0.85;
    } else {
        transformCar[3][0] = -0.75;
    }

    glm::mat4 rotation = getRotationMatrixZAxis( angle );
    rotation *= transformCar;

    drawCircle( color, rotation , g_ViewMatrix, g_ProjectionMatrix );
}

void drawScene(int scene, float runTime) {

    float angle1 = -2.0f*M_PI*runTime/60.0f;

	if (scene != 4) {

        // =====================================================
        // static camera for programming exercise part c:
        // Add your code here:
        // =====================================================


        // =====================================================
        // End Exercise code
        // =====================================================

	} else {

        // rotate around track for the other parts, looking at the center:
        glm::vec3 pos = glm::vec3(1.5f*sin(runTime), 1.5f*cos(runTime), 1.0f );
        g_ViewMatrix = lookAt( pos, glm::vec3(0,0,0)-pos, glm::cross(glm::cross(glm::vec3(0,0,0)-pos,glm::vec3(0,0,1)),glm::vec3(0,0,0)-pos) );

	}

    if (scene == 5) {

        float height = -sin(angle1*2.0) * 0.1;


        // =====================================================
        // Moving camera for programming exercise part e:
        // Add your code here:
        // =====================================================


        // =====================================================
        // End Exercise code
        // =====================================================
    }

	
    // Draw Track (Border and inner track marks)
    drawTrack();

    // Draw finish line
    drawFinish(); 

    // Draw Grandstand on the left
    drawGrandstand();


    drawCar( angle1, 1, glm::vec3(0,1,0) );

	float angle2 = 0.5 * -2.0f*M_PI*runTime/60.0f;
	drawCar( angle2, 2, glm::vec3(1,1,0) );

	if (scene == 3) {
		// optional bonus fun :-D

		// state:
		static float carPosition = 0.0f;
		static float lastRunTime = runTime;
		static float carSpeed = 0.0f;
		static int   carLane = 1;
		
		static float spectatorPos   = 0.9f;
		static bool  spectatorAlive = true;
		static float spectatorDirection = -1.0f;
		
		float timeDiff = runTime - lastRunTime;
		
		// car control:
		if (arrowKeyUpPressed)   carSpeed +=      timeDiff;
		if (arrowKeyDownPressed) carSpeed -= 2.0f*timeDiff;
		carSpeed = glm::clamp( carSpeed, 0.0f, 1.0f );
		
		if (arrowKeyRightPressed) carLane = 2;
		if (arrowKeyLeftPressed)  carLane = 1;
		
		carPosition += -2.0f * M_PI * timeDiff*0.5 * carSpeed;
		if (carPosition < -2.0 * M_PI) carPosition += 2.0 * M_PI;
		
		// car rendering:
		drawCar( carPosition, carLane, glm::vec3(1,0,0) );
		
		// car crash detection:
		float distToCar;
		float crashPos;
		if (carLane == 1) {
            distToCar = abs( carPosition - angle1);
            crashPos  = 0.5*(carPosition + angle1);
		} else {
			distToCar = abs( carPosition - angle2);
			crashPos  = 0.5*(carPosition + angle2);
		}
		if (distToCar < 0.24f) {
			// crash!
			carSpeed = 0.0f;
			
			// Start with identity
			glm::mat4 crash;

			// Scale second car to fit onto track
			crash[0][0] *= 0.02 + 0.5 * (0.24f-distToCar);
			crash[1][1] *= 0.02 + 0.5 * (0.24f-distToCar);
            crash[2][2] *= 0.02 + 0.5 * (0.24f-distToCar);

			if (carLane == 1) {
				crash[3][0] = -0.85;
			} else {
				crash[3][0] = -0.75;
			}
            glm::mat4 rotation = getRotationMatrixZAxis( crashPos );
			rotation *= crash;

            drawCircle( glm::vec3(0.8,0.8,0.8), rotation , g_ViewMatrix, g_ProjectionMatrix);
        }
		
		float spectatorSize = 0.025;
		float distToSpectator = abs( carPosition );
		if (distToSpectator < 0.12f) {
			if ((carLane == 1) && (abs(spectatorPos+0.85)<0.75*spectatorSize) ) {
				spectatorAlive = false;
			}
			if ((carLane == 2) && (abs(spectatorPos+0.75)<0.75*spectatorSize) ){
				spectatorAlive = false;
			}
		}


        glm::mat4 transformSpectator;

		// spectator rendering:
		glm::vec3 spectatorColor = glm::vec3(0.8,0.2,1.0);
		if (spectatorAlive) {
			spectatorPos += 0.03f*timeDiff*spectatorDirection;
			if (spectatorPos <= -0.9f) {
				spectatorPos = -0.9f;
				spectatorDirection = 1.0f;
			}
			if (spectatorPos >= -0.7f) {
				spectatorPos = -0.7f;
				spectatorDirection = -1.0f;
			}           
            transformSpectator[2][2] = 0.06;
		} else {
			spectatorColor = glm::vec3(0.8,0.0,0.0);
			spectatorSize  *= 2.0f;
            transformSpectator[2][2] = 0.005;
		}
		

		transformSpectator[0][0] = spectatorSize;
		transformSpectator[1][1] = spectatorSize;
		transformSpectator[3][0] = spectatorPos;
		
        drawCircle( spectatorColor, transformSpectator , g_ViewMatrix, g_ProjectionMatrix);
		
		lastRunTime = runTime;
	}
}

void initCustomResources()
{
}

void deleteCustomResources()
{
}

