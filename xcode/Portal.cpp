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
	//Just ignore the camera for now. TODO: fully implement portal clipping
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transformation.mat);

	portalObj->draw();
	for(int i=0; i<objs.size(); i++)
		objs[i]->draw();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Portal::setSize(float w, float h){
	width = w;
	height = h;
}

struct Vec2 Portal::getSize(){
	Vec2 size(width, height);
	return size;
}

struct Vec3 Portal::getSW(){
	return pos;
}

struct Vec3 Portal::getSE(){
	struct Vec3 ret = pos;
	vecAdd(&ret, width, 0, 0);
	return ret;
}

struct Vec3 Portal::getNW(){
	struct Vec3 ret = pos;
	vecAdd(&ret, 0, height, 0);
	return ret;
}

struct Vec3 Portal::getNE(){
	struct Vec3 ret = pos;
	vecAdd(&ret, width, height, 0);
	return ret;
}

void Portal::setPos(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;

	transformation.mat[12] = x;
	transformation.mat[13] = y;
	transformation.mat[14] = z;
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

bool Portal::in(struct Vec3* posIn){
	return posIn->x >= pos.x && posIn->x <= pos.x+width && 
			posIn->y >= pos.y && posIn->y <= pos.y+height;
}

MAZEorientation Portal::getOrientation(){
	int verticalVote = (neighbors[0] >= 0) + (neighbors[1] >= 0);
	int horizontalVote = (neighbors[2] >= 0) + (neighbors[3] >= 0);

	return horizontalVote > verticalVote ? HORIZONTAL : VERTICAL;
}