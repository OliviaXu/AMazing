
uniform vec3 lookPosIn;
uniform vec3 motionNormalIn;
uniform float timeIn;

// These are the "input" to our shader.  They are read from the vertex
// arrays that we specified in the C++ code.
attribute vec3 positionIn;
attribute vec2 texcoordIn;
attribute vec3 normalIn;

// These are the "output" values of our shader.  OpenGL will interpolate
// these for us (good riddance project 2!)
varying vec2 texcoord;
varying vec3 normal;
varying vec3 eyePosition;
varying vec3 worldPos;

void main() {
	
	vec4 worldPosTemp = gl_TextureMatrix[0] * gl_ModelViewMatrix * vec4(positionIn, 1);
	//worldPosTemp = worldPosTemp + 0.05 * sin(angle + worldPosTemp.z/5.f) * vec4(motionNormalIn, 1);
	worldPos = worldPosTemp.xyz;
	vec4 eyeTemp = gl_TextureMatrix[1] * worldPosTemp;
	eyePosition = eyeTemp.xyz;


	// Transform the normal, just like in Assignment 2.
	//normal = gl_NormalMatrix * normalIn;
	float angle = timeIn*1.2;
	normal = gl_NormalMatrix * normalIn;
	/*vec4 normalTemp = transpose(gl_TextureMatrix[1]) * vec4(normal, 1);
	normalTemp = normalTemp + 0.5 * sin(angle+worldPos.z/5.0) * vec4(motionNormalIn, 1);
	normalTemp = transpose(gl_TextureMatrix[0]) * vec4(normal, 1);
	normal = normalTemp.xyz;*/


/*	// Transform the vertex to get the eye-space position of the vertex
	vec4 eyeTemp = gl_ModelViewMatrix * vec4(positionIn, 1);
	
	//eyeTemp = eyeTemp + 0.05 * sin(angle) * vec4(normal, 1);
	

	eyePosition = eyeTemp.xyz;

	worldPos = (gl_TextureMatrix[0] * eyeTemp).xyz;*/
	

	// Transform again to get the clip-space position.  The gl_Position
	// variable tells OpenGL where the vertex should go.
	gl_Position = gl_ProjectionMatrix * eyeTemp;

	// Just copy the texture coordinates
	texcoord = texcoordIn;
}
