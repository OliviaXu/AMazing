#include "Wall.h"
#include "Shader.h"
#include "Util.h"

using namespace std;

Wall::Wall(){

};

Wall::~Wall(){

}

void Wall::passShaderParam(const aiMesh *mesh){
	GameObject::passShaderParam(mesh);
	setTan(mesh);
	setNDTex();
}

void Wall::setTan(const aiMesh *mesh) {
	GLuint shaderID = shader->programID();
	GLint tan = glGetAttribLocation(shaderID, "tangent");
	glEnableVertexAttribArray(tan);
	glVertexAttribPointer(tan, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTangents);
	GLint bitan = glGetAttribLocation(shaderID, "binormal");
	glEnableVertexAttribArray(bitan);
	glVertexAttribPointer(bitan, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mBitangents);
}

void Wall::setNormalTex(const sf::Image *normalTex){
	this->normalTex = normalTex;
	
	GL_CHECK(glActiveTexture(GL_TEXTURE2));
	GLuint ntexture;
	glGenTextures( 1, &ntexture );
	glBindTexture( GL_TEXTURE_2D,ntexture );
	//} //always pass in texture even if it is purely blank or black
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );//?need it?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,normalTex->GetWidth(),normalTex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,this->normalTex->GetPixelsPtr());
	
	}

void Wall::setDepthTex(const sf::Image *depthTex){
	this->depthTex = depthTex;
	GL_CHECK(glActiveTexture(GL_TEXTURE3));
	GLuint dtexture;
	glGenTextures( 1, &dtexture );
	glBindTexture( GL_TEXTURE_2D,dtexture);
	//if(stex)
		//depthTex->Bind();
	//else
	//	default_tex.Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,depthTex->GetWidth(),depthTex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,this->depthTex->GetPixelsPtr());
}

void Wall::setNDTex(){
	GLuint shaderID = shader->programID();
	GLint nMap = GL_CHECK(glGetUniformLocation(shaderID, "normalMap"));

	/*
	GL_CHECK(glActiveTexture(GL_TEXTURE2));
	GLuint ntexture;
	glGenTextures( 1, &ntexture );
	glBindTexture( GL_TEXTURE_2D,ntexture );
	//if(normalTex){
	//	normalTex->Bind();
	//}
	//else{
		//default_tex.Bind();
	//} //always pass in texture even if it is purely blank or black
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );//?need it?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,normalTex->GetWidth(),normalTex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,normalTex->GetPixelsPtr());
	*/
	GL_CHECK(glUniform1i(nMap, 2)); // The diffuse map will be GL_TEXTURE0

	GLint dMap = GL_CHECK(glGetUniformLocation(shaderID, "depthMap"));
	
	/*
	GL_CHECK(glActiveTexture(GL_TEXTURE3));
	GLuint dtexture;
	glGenTextures( 1, &dtexture );
	glBindTexture( GL_TEXTURE_2D,dtexture );
	//if(stex)
		//depthTex->Bind();
	//else
	//	default_tex.Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,depthTex->GetWidth(),depthTex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,depthTex->GetPixelsPtr());
	*/
	GL_CHECK(glUniform1i(dMap,3));
}