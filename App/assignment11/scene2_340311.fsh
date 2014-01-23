#version 150

in vec3 vColor;

out vec4 oFragColor;

void main() {

    // Set constant color for splat
    oFragColor = vec4(vColor, 1.0f);
}
