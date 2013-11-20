
#version 150

in vec4 aPosition;
in vec3 aColor;

out vec3 Color;

uniform mat4 model;
uniform mat4 proj;

void main() {
    Color = aColor;
    gl_Position = proj * model * aPosition;
}
