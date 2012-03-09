#include "Portal.h"

Portal::Portal(){
	neighbors[0] = -1;
	neighbors[1] = -1;
	neighbors[2] = -1;
	neighbors[3] = -1;
	width = 0;
	height = 0;
}

Portal::~Portal(){

}

void Portal::draw(Camera* camera){
	//Just ignore the camera right now. TODO: fully implement portal clipping
	portalObj->draw();
	for(int i=0; i<objs.size(); i++)
		objs[i]->draw();
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