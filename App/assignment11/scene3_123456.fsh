#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec3 vColor;
in float vSplatSize;

uniform mat4 uProjectionMatrix;
uniform mat4 uInverseProjectionMatrix;
uniform float uWindowWidth;
uniform float uWindowHeight;
uniform float uNear;
uniform float uFar;

out vec4 oFragColor;

void main() {

    // =======================================================================
    // =======================================================================
    // Assignment code:
    // Part C:
    // =======================================================================
    // =======================================================================
    

    // Replace with your code
    oFragColor = vec4(1.0f,0.0f,0.0f, 1.0f);
 
    
    // =======================================================================
    // End assignment code
    // =======================================================================
}
