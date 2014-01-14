#version 150

// Get in the transformed positions and normals
// now in eye-space
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D uTexture;

uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform mat3 uAmbientMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

uniform float uMixTextures;
uniform vec3 uBlendColor;
uniform int  uBoundingObject;

out vec4 oFragColor;

/*
 * Lighting model: Phong/Blinn lighting per vertex
 * Shading Model: Average of colors of incident vertices per triangle
 * Assume attenuation = 1;
 */
vec3 ambient() {

    return uLightColor * uAmbientMaterial;
}

vec3 diffuse() {
	vec3 texColor = uBlendColor * vec3(texture(uTexture, vTexCoord).rgb);
    vec3 lp = normalize(uLightPosition - vec3(vPosition));
    vec3 n = normalize(vNormal);
    float cosAngle = max(dot(lp, normalize(vNormal)), 0);
    return vec3((uLightColor*texColor) * cosAngle);
}

vec3 specular() {

    vec3 bisector = normalize(normalize(-vec3(vPosition)) + normalize((uLightPosition - vec3(vPosition))));

    float cosAngle = max(dot(bisector, normalize(vNormal)), 0);

    return uLightColor * uSpecularMaterial * pow(cosAngle, uSpecularityExponent);
}

void main() {

    vec3 fragColor;
    if (uBoundingObject == 0) {
    	fragColor = ambient() + diffuse() + specular();
	} else {
		fragColor = uBlendColor;
	}
    oFragColor = vec4(fragColor, 1.0f);
}
