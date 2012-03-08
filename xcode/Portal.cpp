#include "Portal.h"

Portal::Portal(){

}

Portal::~Portal(){

}

void Portal::draw(Camera* camera){
	
}

int *Portal::getNeighbors(){
	return neighbors;
}

void Portal::addObject(GameObject *obj){
	objs.push_back(obj);
}

void Portal::setPortalObject(GameObject *obj){
	portalObj = obj;
}

bool Portal::in(struct Vec3* pos){
	return true;
}