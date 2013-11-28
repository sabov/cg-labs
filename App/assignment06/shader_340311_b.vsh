#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightSpotLightFactor;

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

    vec3 global = vec3(0, -3, -5);

    vec3 n = vec3(inverse(transpose(Mmv)) * vec4(aNormal, 0));
    vec3 wl = normalize(uLightPosition - p);
    vec3 wv = normalize(v - p);
    vec3 h = normalize(wl + wv);

    float spot = pow(dot(normalize(global - uLightPosition), normalize(p -
                    uLightPosition)), uLightSpotLightFactor);

    float diffuse = dot(n, wl);
    float specular = pow(max(0, dot(n, h)), uSpecularityExponent);

    vColor = vec3(uAmbientMaterial * uLightColor +
                        spot * (uDiffuseMaterial * uLightColor * diffuse +
                        uSpecularMaterial * uLightColor * specular));

}
