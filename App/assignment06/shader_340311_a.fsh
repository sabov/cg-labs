#version 150

in vec3 vColor;

out vec4 oFragColor;

void main() {
    oFragColor = vec4( vColor, 1);

}
