#version 150

in vec4 vPosition;
in vec3 vNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uLightSpotLightFactor;

uniform mat3 uAmbientMaterial;
uniform mat3 uDiffuseMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

out vec4 oFragColor;

void main() {

    mat4 Mmv = uModelViewMatrix;
    vec3 p = vec3(Mmv * vPosition);
    vec3 v = vec3(0, 0, 0);

    vec3 global = vec3(vec4(0,0,0,0) * Mmv);

    vec3 n = normalize(vec3(inverse(transpose(Mmv)) * vec4(vNormal, 0)));
    vec3 wl = normalize(uLightPosition - p);

    vec3 r = 2 * n * n * (uLightPosition - p) - (uLightPosition - p);

    float spot = pow(dot(normalize(global - uLightPosition), normalize(p -
                    uLightPosition)), uLightSpotLightFactor);

    float diffuse = dot(n, wl);
    float specular = pow(max(0, dot(r, normalize(v - p))), uSpecularityExponent);

    oFragColor = vec4(uAmbientMaterial * uLightColor +
                        spot * (uDiffuseMaterial * uLightColor * diffuse +
                        uSpecularMaterial * uLightColor * specular), 1);
}
