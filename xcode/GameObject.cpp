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

void GameObject::setShader(int iShader){
	
}

void GameObject::setMass(int mass){
	this->mass = mass;
}

void GameObject::setPos(Vec3 &pos){
	//this->pos = pos;
}

void setClass(char *className){

}

struct Vec3 &GameObject::getPos(){
    Vec3 res = Vec3(0, 0, 0);
	return res;
}

void GameObject::setClass(char *className){

}