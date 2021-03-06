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
    pos.xyz *= 2.0/3.0*abs(cos(time)) + 1.0/3.0;
	gl_Position = projectionMatrix * modelViewMatrix * pos;
    // End of your code here

    vColor  = color;
}
