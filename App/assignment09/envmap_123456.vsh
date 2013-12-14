#version 150

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uInvTranspModelMatrix;
uniform mat4 uInvTranspModelViewMatrix;

out vec4 vEyePosition; // the vertex in eyeSpace
out vec3 vWorldPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vWorldNormal;

void main() {

	vWorldPosition = (uModelMatrix * aPosition).xyz;
    // Transform to eye-space
    vEyePosition = uViewMatrix * vec4(vWorldPosition, 1.0);
    
    vNormal = vec3(uInvTranspModelViewMatrix * vec4(aNormal, 0.0f)); // eye normal
	vWorldNormal = mat3(uInvTranspModelMatrix) * aNormal;
	
    vTexCoord = aTexCoord;

    gl_Position = uProjectionMatrix * vEyePosition;
}
