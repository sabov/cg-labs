#version 150

in vec3 Color;
out vec4 oFragColor;

void main() {
    oFragColor = vec4( Color, 1);
}
