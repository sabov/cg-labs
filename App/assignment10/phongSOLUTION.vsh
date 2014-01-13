#version 150

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uInvTranspModelViewMatrix;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {

    // Transform to eye-space
    vPosition = uModelViewMatrix * aPosition;
    vNormal = vec3(uInvTranspModelViewMatrix * vec4(aNormal, 0.0f));

    vTexCoord = aTexCoord;

    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
}
