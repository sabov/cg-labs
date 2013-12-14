#version 150

in vec4 aPosition;
in vec2 aTexCoord;

uniform mat4 uMVP;

out vec2 vTexCoord;

void main() {
	vTexCoord = aTexCoord;
    gl_Position = uMVP * aPosition;
}
