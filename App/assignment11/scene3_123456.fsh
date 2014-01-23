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

#define ZNEAR uNear
#define ZFAR uFar

#define A (ZNEAR + ZFAR)
#define B (ZNEAR - ZFAR)
#define C (2.0 * ZNEAR * ZFAR)
#define D (ndcPos.z * B)
#define ZEYE -(C / (A + D))

void main() {

    // =======================================================================
    // =======================================================================
    // Assignment code:
    // Part C:
    // =======================================================================
    // =======================================================================
    
    vec2 windowSize = vec2(uWindowWidth, uWindowHeight);


    vec3 ndcPos;
    ndcPos.xy = gl_FragCoord.xy / windowSize;
    ndcPos.z = gl_FragCoord.z;
    ndcPos -= 0.5;
    ndcPos *= 2.0;
    vec4 clipPos;
    clipPos.w = -ZEYE;
    clipPos.xyz = ndcPos * clipPos.w;
    vec4 p = uInverseProjectionMatrix * clipPos;

    float depth = -vNormal.x/vNormal.z * vPosition.x - vNormal.y/vNormal.z * vPosition.y;

    if(pow(p.x - vPosition.x, 2) + pow(p.y - vPosition.y, 2) > pow(vSplatSize, 2)) {
        discard;
    }
    oFragColor = vec4(vColor, 1.0f);

    // Replace with your code
 
    
    // =======================================================================
    // End assignment code
    // =======================================================================
}
