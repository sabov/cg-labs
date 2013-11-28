#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform mat3 uAmbientMaterial;
uniform mat3 uDiffuseMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

out vec3 vColor;

void main() {

    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;

    mat4 Mmv = uModelViewMatrix;
    vec3 p = vec3(Mmv * aPosition);
    vec3 v = vec3(0, 0, 0);

    vec3 n = vec3(inverse(transpose(Mmv)) * vec4(aNormal, 0.0f));
    vec3 wl = normalize(uLightPosition - p);
    vec3 wv = normalize(v - p);
    vec3 h = normalize(wl + wv);

    float diffuse = max(0.0f, dot(n, wl));
    float specular = pow(max(0.0f, dot(h, n)), 2);

    vColor = vec3(uAmbientMaterial * uLightColor +
                        uDiffuseMaterial * uLightColor * diffuse +
                        uSpecularMaterial * uLightColor * specular);

}
