#version 150

in vec4 aPosition;
in vec3 aColor;
in float aSplatSize;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

uniform float uScaleToScreen;

out vec3 vColor;

void main() {

    // Pass color to fragment shader
    // Note: No bi-linear interpolation will be applied
    // since we're rendering points instead of triangles
    vColor = aColor;
    
    // =======================================================================
    // =======================================================================
    // Assignment code:
    // Part B:
    // =======================================================================
    // =======================================================================

    // Replace with your code
    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
    
    // =======================================================================
    // End assignment code
    // =======================================================================
}
