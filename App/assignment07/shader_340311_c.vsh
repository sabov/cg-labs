#version 150

in vec4 positionS;
in vec4 positionK;
in vec3 colorS;
in vec3 colorK;

out vec3 vColor;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform float time;

void main() {

    // Add your code here

	vec4 pos = positionK;
	pos =  positionK + (positionS - positionK) * abs(cos(time)); 
	//pos.y =   positionK.y + (positionS.y - positionK.y) * abs(cos(time)); 
	//pos.z =   positionK.z + (positionS.z - positionK.z) * abs(cos(time)); 
	gl_Position = projectionMatrix * modelViewMatrix * pos;
    
    vColor  = colorK + (colorS - colorK) * abs(cos(time));;

    // End of your code
    
}

