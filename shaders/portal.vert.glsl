#version 120
uniform vec3 lookPosIn;
uniform float timeIn;

// These are the "input" to our shader.  They are read from the vertex
// arrays that we specified in the C++ code.
attribute vec3 positionIn;
attribute vec2 texcoordIn;
attribute vec3 normalIn;
attribute vec3 tangentIn;
attribute vec3 bitangentIn;

// These are the "output" values of our shader.  OpenGL will interpolate
// these for us (good riddance project 2!)
varying vec2 texcoord;
varying vec3 normal;
varying vec3 eyePosition;
varying vec3 worldPos;

attribute float alphaIn;
varying float alpha_blend;

void main() {
	// Transform the normal, just like in Assignment 2.
	//normal = normalize(gl_NormalMatrix * normalIn);
    	float twopi=6.28;
    	float w=7;//frequency
    	float a=0.008;
    	float b=.5;//center
    	float c=.2;
    	float delta=0.0001;

	//texcoord = gl_MultiTexCoord0.xy;
	vec2 texCenter = vec2(0.5, 0.5);
	float dist=distance(texcoordIn, texCenter);

    	float h_o=a*exp(-dist*dist/8)*cos(twopi*w*dist-timeIn);
    	vec3 disp=normal*h_o;

	vec3 pos = positionIn + disp;
	vec4 eyeTemp = gl_ModelViewMatrix * vec4(positionIn, 1);
	eyePosition = eyeTemp.xyz;
	worldPos = (gl_TextureMatrix[0] * eyeTemp).xyz;

	normal = normalize(normalIn);
	float distu = distance(vec2(texcoordIn.x+delta, texcoordIn.y), texCenter);
	float distv = distance(vec2(texcoordIn.x, texcoordIn.y+delta), texCenter);

    	float h_u=a*exp(-distu*distu/8)*cos(twopi*w*distu-timeIn);
    	float h_v=a*exp(-distv*distv/8)*cos(twopi*w*distv-timeIn);
    	float dhu=(h_u-h_o)/delta;
    	float dhv=(h_v-h_o)/delta;
    	vec3 T1=normalize(tangentIn + dhu*normal);
    	vec3 T2=normalize(bitangentIn + dhv*normal);
    	normal= -cross(T1,T2);
	normal = gl_NormalMatrix * normal;

	// Transform again to get the clip-space position.  The gl_Position
	// variable tells OpenGL where the vertex should go.
	gl_Position = gl_ProjectionMatrix * eyeTemp;

	// Just copy the texture coordinates
	texcoord = texcoordIn;
    
    alpha_blend = alphaIn;
}
