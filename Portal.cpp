#include "Portal.h"

Portal::Portal(){
	neighbors[0] = -1;
	neighbors[1] = -1;
	neighbors[2] = -1;
	neighbors[3] = -1;

	doorStatus[0] = 0;
	doorStatus[1] = 0;
	doorStatus[2] = 0;
	doorStatus[3] = 0;

	width = 0;
	height = 0;
}

Portal::~Portal(){

}

bool Portal::cullDraw(struct MAZEmat *projMat, struct MAZErectangle rec, vector<Portal *> portals){
	MAZEmat finalProjMat;
	multMat(projMat, &transformation, &finalProjMat);

	//TODO: implement BV for objects
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transformation.mat);

	portalObj->draw();
	for(int i=0; i<objs.size(); i++)
		objs[i]->draw();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	for(int i=0; i<4; i++){
		portals[neighbors[i]]->
	}
	return true;
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
	vecAdd(&ret, 0, 0, height);
	return ret;
}

struct Vec3 Portal::getNE(){
	struct Vec3 ret = pos;
	vecAdd(&ret, width, 0, height);
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
			posIn->z >= pos.z && posIn->z <= pos.z+height;
}

MAZEorientation Portal::getOrientation(){
	int verticalVote = (neighbors[0] >= 0) + (neighbors[1] >= 0);
	int horizontalVote = (neighbors[2] >= 0) + (neighbors[3] >= 0);
	if(verticalVote == horizontalVote || (verticalVote == 1 && horizontalVote == 1))
		return JOINT;
	return horizontalVote > verticalVote ? HORIZONTAL : VERTICAL;
}