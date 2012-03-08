#include "GameObject.h"

GameObject::GameObject(){

}

GameObject::~GameObject(){
	
}

void GameObject::draw(){

}

void GameObject::updatePhysicalProperty(Vec3 &newPos, 
	float newMass, float newVel, float newAcc){

}

void GameObject::setPortal(int iPortal){
	this->iPortal = iPortal;
}

void GameObject::setShader(Shader *shader){
	this->shader = shader;
}

void GameObject::setMass(int mass){
	this->mass = mass;
}

void GameObject::setPos(Vec3 &posIn){
	pos = posIn;
}

struct Vec3 &GameObject::getPos(){
    Vec3 res = Vec3(0, 0, 0);
	return res;
}

void GameObject::setClass(char *className){

}

void GameObject::setModel(const aiScene *model){
	this->model = model;
}