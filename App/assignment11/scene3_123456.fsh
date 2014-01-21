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
    
    vec2 viewport = vec2(uWindowWidth, uWindowHeight);
    vec4 ndcPos;
    ndcPos.xy = ((2.0 * gl_FragCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1;
    ndcPos.z = (2.0 * gl_FragCoord.z - uNear - uFar) / (uFar - uNear);
    ndcPos.w = 1.0;
     
    vec4 clipPos = ndcPos / gl_FragCoord.w;
    vec4 eyePos = uInverseProjectionMatrix * clipPos;

    // Replace with your code
    oFragColor = vec4(vColor, 1.0f);
 
    
    // =======================================================================
    // End assignment code
    // =======================================================================
}
