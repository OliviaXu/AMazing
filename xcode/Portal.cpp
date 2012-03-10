#include "Portal.h"
using namespace std;

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

bool Portal::cullDraw(struct MAZEmat *projMat, struct MAZErectangle &rec, vector<Portal *> *portals){
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
	
	int posMap[4] = {1, 0, 3, 2};
	for(int i=0; i<4; i++){
		if(neighbors[i] < 0)
			continue;
		struct Vec3 p[4];
		float left = rec.left, right = rec.left+rec.width, 
			top = rec.bottom+rec.height, bottom = rec.bottom;
		for(int j=0; j<4; j++){
			matMultVec(&finalProjMat, &doorPoints[i][j], p+j);
			if(p[j].x > left)
				left = p[j].x;
			if(p[j].x < right)
				right = p[j].x;

			if(p[j].z < top)
				top = p[j].z;
			if(p[j].z > bottom)
				bottom = p[j].z;
		}
		MAZErectangle neighborRect;
		neighborRect.left = left;
		neighborRect.bottom = bottom;
		neighborRect.height = top-bottom;
		neighborRect.width = right-left;

		if(neighborRect.height <= 0 || neighborRect.width <= 0)
			continue;
		
		//Door status switched. If it is open from this cell to its
		//neighbor, we might visit that neighbor. When we are visiting
		//the neighbor, we will close the door and would not visit this
		//cell. Is it still possible that we might loop back to visited portal?
		int *nDoorStatus = (*portals)[neighbors[i]]->doorStatus;
		nDoorStatus[posMap[i]] = nDoorStatus[posMap[i]] ? 0 : 1;
		doorStatus[i] = nDoorStatus[posMap[i]];
		//If the door is open, visit it.
		//NOTE: Theorectically it's possible that a single object in a cell
		//can be rendered several times from different doors. TODO: I might add some
		//mask to handle this problem in the future. But, for this maze game,
		//this scenario seems unlikely. So ignore it for now.
		if(nDoorStatus[posMap[i]])
			(*portals)[neighbors[i]]->cullDraw(projMat, neighborRect, portals);
	}
	return true;
}

void Portal::setSize(float w, float h){
	width = w/25.4;
	height = h/25.4;

	struct Vec3 nw, ne, sw, se;
	nw = getNW();
	ne = getNE();
	sw = getSW();
	se = getSE();

	float maxY = (w+h)*10;

	setVec3(&nw, &doorPoints[0][0]);
	setVec3(&ne, &doorPoints[0][1]);
	setVec3(&sw, &doorPoints[1][0]);
	setVec3(&se, &doorPoints[1][1]);
	setVec3(&nw, &doorPoints[2][0]);
	setVec3(&sw, &doorPoints[2][1]);
	setVec3(&ne, &doorPoints[3][0]);
	setVec3(&se, &doorPoints[3][1]);

	nw.y = maxY;
	ne.y = maxY;
	sw.y = maxY;
	se.y = maxY;
	setVec3(&nw, &doorPoints[0][0]);
	setVec3(&ne, &doorPoints[0][1]);
	setVec3(&sw, &doorPoints[1][0]);
	setVec3(&se, &doorPoints[1][1]);
	setVec3(&nw, &doorPoints[2][0]);
	setVec3(&sw, &doorPoints[2][1]);
	setVec3(&ne, &doorPoints[3][0]);
	setVec3(&se, &doorPoints[3][1]);
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

	transformation.mat[12] = -x;
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