#version 150

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord; // the texture coordinates, you might get a warning from the shader compiler that this isn't present. 
// That's because it isn't used (yet) and gets removed from the shader optimiser. Don't mind that, the warning will go 
// away as soon as you use these values!

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uInvTranspModelViewMatrix;

out vec4 vPosition;
out vec3 vNormal;

void main() {

    // Transform to eye-space
    vPosition = uModelViewMatrix * aPosition;
    vNormal = vec3(uInvTranspModelViewMatrix * vec4(aNormal, 0.0));
	
    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
}
