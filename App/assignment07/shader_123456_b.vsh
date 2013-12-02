#version 150

uniform float time;

in vec4 position;
in vec3 color;

out vec3 vColor;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
    vec4 pos = position;

    // Add your code here

    // End of your code here

    vColor  = color;
}
