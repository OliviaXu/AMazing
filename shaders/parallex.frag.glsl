#version 120
// This is a texture sampler.  It lets you sample textures!  The keyword
// "uniform" means constant - sort of.  The uniform variables are the same
// for all fragments in an object, but they can change in between objects.
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
//I will use diffuse Map and specularMap first to do the job of normal map

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

float inter(vec2 dp,vec2 ds){
  int linear_steps=10;
  int bi_steps=5;
  float depth_step=0.01;//every time 
  float size=depth_step;
  float dep=0.0;
  float best=1.0;//from the surface down
  int i=0;
  for(i;i<linear_steps-1;i++){
    dep=dep+size;
    vec4 t=texture2D(specularMap,dp+ds*dep);
    if(best>0.996)
    if(!(dep<t.r))
    //maybe try to see whether w has value.....
       best=dep;
         
  }
  dep=best;
  int j=0;
  for(j;j<bi_steps-1;j++){
    size=size*0.5;
    vec4 t=texture2D(specularMap,dp+ds*dep);
    if(!(dep<t.r)){
      best=dep;
      dep=dep-2*size;
    }
    dep=dep+size;
  }
  return best;
}
void main() {

  float depth=0.0;//don't know what exactly
  float tile=1.0;//don't know this as well...
  
	vec3 V = normalize(eyePosition);
    vec3 tanv=normalize(vec3(dot(V,tan),dot(V,bin),dot(normal,-V)));
    vec2 ds=tanv.xy*depth/tanv.z;
    vec2 dp=texcoord*tile;
    float d=inter(dp,ds);
    vec2 uv=dp+ds*d;
    vec3 tn=texture2D(diffuseMap,uv).xyz;
    //maybe color texture
    tn=tn*2.0-1.0;
    tn.xyz=normalize(tn.x*tan+tn.y*bin+tn*normal);
    //light
    vec3 pos=eyePosition+V*d*tanv.z;
    vec3 ldir=normalize(pos-gl_LightSource[0].position.xyz);
    
    //need to do depth correction???
    	float Rd = max(0.0, dot(-ldir,tn));
      float Rs = pow(max(0.0, dot(normalize(-ldir-V), tn)), alpha);
      //why the diffuse is like this?
     
     
     
    //do shadow
    dp+=ds*d;
    vec3 lightTex=normalize(vec3(dot(ldir,tan),dot(ldir,bin),dot(normal,-ldir)));
    ds=lightTex.xy*depth/lightTex.z;
    dp-=ds*d;
    float dl=inter(dp,ds);
    if(dl<d-0.05){
      //in shadow
      Rd*=dot(gl_LightSource[0].ambient.rgb,vec3(1.,1.,1)*0.3333);
      Rs=0;
    }
    
    
    
    //with no diffuse map at this moment
     vec3 diffuse = Rd * Kd * gl_LightSource[0].diffuse.rgb;
     vec3 specular = Rs * Ks * gl_LightSource[0].specular.rgb;
     vec3 ambient = Ka * gl_LightSource[0].ambient.rgb;
     
    // This actually writes to the frame buffer
    vec3 color = diffuse + specular + ambient;
    if(color.r>1) color.r=1;
    if(color.g>1) color.g=1;
    if(color.b>1) color.b=1;
    gl_FragColor = vec4(color, 1);
    //gl_FragColor = vec4(texture2D(specularMap,texcoord).xyz,1);
     //gl_FragColor = vec4(tan,1);

}


