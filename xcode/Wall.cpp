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
}

void Wall::setDepthTex(const sf::Image *depthTex){
	this->depthTex = depthTex;
}