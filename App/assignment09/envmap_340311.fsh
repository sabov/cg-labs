#version 150

// Get in the transformed positions and normals
// now in eye-space
in vec4 vEyePosition;
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vWorldNormal;

uniform sampler2D uMaterialDiffuse;
uniform sampler2D uTextureSphereMap;
uniform sampler2D uTextureCubeMap;

uniform mat4 uModelMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform float uSpecularityExponent;
uniform vec3 uCameraPosition;

uniform bool uUseCubeMapping;
uniform bool uEnvironmentOnly;


// =======================================================================
// =======================================================================
// Assignment code:
// you can add more uniforms and varyings if you need them!
// =======================================================================
// =======================================================================


// =======================================================================
// End assignment code
// =======================================================================

out vec4 oFragColor;

/*
 * Lighting model: Phong/Blinn lighting per vertex
 * Shading Model: Average of colors of incident vertices per triangle
 * Assume attenuation = 1;
 */
vec3 ambient() {

    vec3 texColor = texture(uMaterialDiffuse, vTexCoord).rgb;
    return uLightColor * texColor * 0.02; //  to damp the ambient color
}

vec3 diffuse() {
	vec3 texColor = texture(uMaterialDiffuse, vTexCoord).rgb;

    vec3 lp = normalize(uLightPosition - vec3(vEyePosition));
    vec3 n = normalize(vNormal);
    float cosAngle = max(dot(lp, normalize(vNormal)), 0);
    return vec3((uLightColor*texColor) * cosAngle);
}

vec3 specular() {
    vec3 bisector = normalize(normalize(-vec3(vEyePosition)) + normalize((uLightPosition - vec3(vEyePosition))));

    float cosAngle = max(dot(bisector, normalize(vNormal)), 0);

    float specularMaterial = 0.5;
    return specularMaterial * uLightColor * pow(cosAngle, uSpecularityExponent);
}
vec3 reflectionColor(vec3 r) {
    vec2 texCoord;
    float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
    texCoord.s = r.x/m + 0.5;
    texCoord.t = r.y/m + 0.5;
    return texture(uTextureSphereMap, texCoord).rgb;
}

void main() {

   vec3 normalInWorldSpace = normalize( vWorldNormal );
   vec3 cameraDir = normalize( vWorldPosition - uCameraPosition );
   vec3 reflectionVector = normalize( reflect( cameraDir, normalInWorldSpace ) );

   vec3 reflectionColor = vec3(0.0);
   vec3 lighingTerm = ambient() + diffuse() + specular();

   // =======================================================================
   // =======================================================================
   // Assignment code:
   // =======================================================================
   // =======================================================================


    if (uUseCubeMapping) {
        vec2 texCoord;//cube map textore dosen't appear :(
        texCoord.s = 0.5;//debug
        texCoord.t = 0.5;
        reflectionColor = texture(uTextureCubeMap, texCoord).rgb;
    } else {
        reflectionColor = reflectionColor(reflectionVector);
    }

    if (uEnvironmentOnly) {
       vec3 fragColor = reflectionColor;
       oFragColor = vec4(fragColor, 1.0f);
    } else {
       vec3 fragColor = lighingTerm + 0.1 * reflectionColor;
       oFragColor = vec4(fragColor, 1.0f);
    }

    // =======================================================================
    // End assignment code
    // =======================================================================
}
