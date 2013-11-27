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

    vec3 l = normalize(uLightPosition);
    vec3 n = normalize(aNormal);
    vec3 v = normalize(vec3(0,0,0));
    vec3 h = normalize(l+v);

    float diffuse = max(dot(l,n), 0);
    float specular = pow(max(0.0,dot(n,h)),4);

    vColor = vec3(uAmbientMaterial +
                        uDiffuseMaterial *  diffuse +
                        uSpecularMaterial * specular * uSpecularityExponent);


}
