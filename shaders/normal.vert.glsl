#version 120
// These are the "input" to our shader.  They are read from the vertex
// arrays that we specified in the C++ code.
attribute vec3 positionIn;
attribute vec2 texcoordIn;
attribute vec3 normalIn;
attribute vec3 tangent; //The inverse tangent to the geometry
attribute vec3 binormal; //The inverse binormal to the geometry

// These are the "output" values of our shader.  OpenGL will interpolate
// these for us (good riddance project 2!)
varying vec2 texcoord;
varying vec3 normal;
varying vec3 tan;
varying vec3 bin;
varying vec3 eyePosition;

void main() {
	// Transform the vertex to get the eye-space position of the vertex
	vec4 eyeTemp = gl_ModelViewMatrix * vec4(positionIn, 1);
	eyePosition = eyeTemp.xyz;

	// Transform again to get the clip-space position.  The gl_Position
	// variable tells OpenGL where the vertex should go.
	gl_Position = gl_ProjectionMatrix * eyeTemp;
	//gl_Position = vec4(0,0,0,1);
	// Transform the normal, just like in Assignment 2.
	normal = gl_NormalMatrix * normalIn;
	tan=gl_NormalMatrix *tangent;
  bin=gl_NormalMatrix *binormal;

	// Just copy the texture coordinates
	texcoord = texcoordIn;
}
