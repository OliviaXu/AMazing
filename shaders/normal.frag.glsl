#version 120
// This is a texture sampler.  It lets you sample textures!  The keyword
// "uniform" means constant - sort of.  The uniform variables are the same
// for all fragments in an object, but they can change in between objects.
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

// Diffuse, ambient, and specular materials.  These are also uniform.
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ka;
uniform float alpha;
// These are values that OpenGL interpoates for us.  Note that some of these
// are repeated from the fragment shader.  That's because they're passed
// across.
varying vec2 texcoord;
varying vec3 normal;
varying vec3 tan;
varying vec3 bin;
varying vec3 eyePosition;

void main() {
	// Normalize the normal, and calculate light vector and view vector
	// Note: this is doing a directional light, which is a little different
	// from what you did in Assignment 2.
	
	vec3 norm = texture2D(normalMap,texcoord).rgb;
  norm=2.*norm;
  norm=norm-1.;
  mat3 tbn;
  tbn[0]=tan;
  tbn[1]=bin;
  tbn[2]=normal;
  vec3 N= normalize(tbn * norm);//change this
	
	
	//vec3 N = normalize(normal);
	vec3 L = normalize(gl_LightSource[0].position.xyz);
	vec3 V = normalize(-eyePosition);
		
	vec3 L1 = normalize(gl_LightSource[1].position.xyz);
	
	// Calculate the diffuse color coefficient, and sample the diffuse texture
	float Rd = max(0.0, dot(L, N));
	vec3 Td = texture2D(diffuseMap, texcoord).rgb;
	//vec3 diffuse = Rd * Kd * Td * gl_LightSource[0].diffuse.rgb;
	vec3 diffuse = Rd * Td * gl_LightSource[0].diffuse.rgb;
	
	float Rd1 = max(0.0, dot(L1, N));
	//diffuse = diffuse + Rd1 * Kd * Td *gl_LightSource[1].diffuse.rgb;
	diffuse = diffuse + Rd1 * Td *gl_LightSource[1].diffuse.rgb;

	// Calculate the specular coefficient
	vec3 R = reflect(-L, N);
	float Rs = pow(max(0.0, dot(V, R)), alpha);
	vec3 Ts = texture2D(specularMap, texcoord).rgb;
	//vec3 specular = Rs * Ks * Ts * gl_LightSource[0].specular.rgb;
	vec3 specular = Rs * Ts * gl_LightSource[0].specular.rgb;
	
	vec3 R1 = reflect(-L1, N);
	float Rs1 = pow(max(0.0, dot(V, R1)), alpha);
	//specular = specular + Rs1 * Ks * Ts * gl_LightSource[1].specular.rgb;
  specular = specular + Rs1 * Ts * gl_LightSource[1].specular.rgb;

	// Ambient is easy
	vec3 ambient = Ka * Td * (gl_LightSource[0].ambient.rgb + gl_LightSource[1].ambient.rgb);
	
	// This actually writes to the frame buffer
	gl_FragColor = vec4(diffuse + specular + ambient, 1);
}
