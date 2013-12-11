#version 150

// Get in the transformed positions and normals
// now in eye-space
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D uTexKiller;
uniform mat4 uModelViewMatrix;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform float uSpecularityExponent;

out vec4 oFragColor;

vec3 getDiffuseMaterial()
{
	// TODO: sample the diffuse material here:
	//return vec3(0.1,1.0,0.1);
    return vec3(texture(uTexKiller, vTexCoord));
}

/*
 * Assume attenuation = 1;
 */
vec3 ambient() {

    return uLightColor * vec3(0.05) * getDiffuseMaterial();
}

vec3 diffuse() {
	vec3 texColor = getDiffuseMaterial();

    vec3 lp = normalize(uLightPosition - vec3(vPosition));
    vec3 n  = normalize(vNormal);
    float cosAngle = max(dot(lp, normalize(vNormal)), 0);
    return vec3((uLightColor*texColor) * cosAngle);
}

vec3 specular() {

    vec3 bisector = normalize(normalize(-vec3(vPosition)) + normalize((uLightPosition - vec3(vPosition))));
    float cosAngle = max(dot(bisector, normalize(vNormal)), 0);

	float specularMaterial = 1.0; // in part C this should be read from a second texture!
    return specularMaterial * uLightColor * max( pow(cosAngle, uSpecularityExponent), 0.0 );
}

void main() {

    vec3 fragColor = ambient() + diffuse() + specular();
    oFragColor = vec4(fragColor, 1.0f);

}
