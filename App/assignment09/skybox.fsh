#version 150

in vec2 vTexCoord;
uniform sampler2DArray uTexture;
out vec4 oFragColor;
uniform int layer;

void main() {
    oFragColor = 0.75*texture( uTexture, vec3(vTexCoord, layer) );
}
