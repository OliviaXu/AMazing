#include "Portal.h"
using namespace std;

Portal::Portal(){
	neighbors[0] = -1;
	neighbors[1] = -1;
	neighbors[2] = -1;
	neighbors[3] = -1;

	//transformation.mat[0] = -1;//negate x axis

	width = 0;
	height = 0;
}

Portal::~Portal(){

}

bool Portal::cullDraw(struct MAZEmat *projviewMat, struct MAZEmat *viewportMat, 
					struct MAZErectangle &rec, const std::vector<Portal *> *portals, set<int *> &visitedEdgeSet){
	MAZEmat finalProjViewMat;
	multMat(projviewMat, &transformation, &finalProjViewMat);

	//TODO: implement BV for objects
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glMultMatrixf(transformation.mat);
	if(!portalObj->isHidden())
		portalObj->draw(portals);
	for(int i=0; i<objs.size(); i++)
		if(!objs[i]->isHidden())
			objs[i]->draw(portals);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	//What this portal looks like in its neighbors eye...
	int posMap[4] = {1, 0, 3, 2};

	for(int i=0; i<4; i++){
		if(neighbors[i] < 0)
			continue;

		//Have we visited this undirected edge? 
		//I know the following code seems a little hack
		//but I can't find any better way...
		Portal *port = (*portals)[neighbors[i]];
		if(visitedEdgeSet.count(port->getNeighbors()+posMap[i]) > 0)
			continue;
		visitedEdgeSet.insert(neighbors+i);

		struct Vec3 p[4];
		float left = MY_FLT_MAX, right = MY_FLT_MIN, top = MY_FLT_MIN, bottom = MY_FLT_MAX;
		float nearC = MY_FLT_MAX, farC = MY_FLT_MIN;
		for(int j=0; j<4; j++){
			struct Vec3 tmp;
			matMultVec3_normalize(&finalProjViewMat, &doorPoints[i][j], &tmp);
			matMultVec3_normalize(viewportMat, &tmp, p+j);
			if(p[j].x < left)
				left = p[j].x;
			if(p[j].x > right)
				right = p[j].x;

			if(p[j].y > top)
				top = p[j].y;
			if(p[j].y < bottom)
				bottom = p[j].y;

			if(p[j].z < nearC)
				nearC = p[j].z;

			if(p[j].z > farC)
				farC = p[j].z;
		}
		if(farC < 0 || nearC > 1)
			continue;

		left = left > rec.left ? left : rec.left;
		right = right < (rec.left+rec.width) ? right : rec.left+rec.width;
		bottom = bottom > rec.bottom ? bottom : rec.bottom;
		top = top < (rec.bottom + rec.height) ? top : rec.bottom+rec.height;
		MAZErectangle neighborRect;
		neighborRect.left = left;
		neighborRect.bottom = bottom;
		neighborRect.height = top-bottom;
		neighborRect.width = right-left;
		//This is necessary for edge condition
		if(neighborRect.height <= -1e-5 || neighborRect.width <= -1e-5)
			continue;
		
		//Door status switched. If it is open from this cell to its
		//neighbor, we might visit that neighbor. When we are visiting
		//the neighbor, we will close the door and would not visit this
		//cell. Is it still possible that we might loop back to visited portal?
		//If the door is open, visit it.
		//NOTE: Theorectically it's possible that a single object in a cell
		//can be rendered several times from different doors. TODO: I might add some
		//mask to handle this problem in the future. But, for this maze game,
		//this scenario seems unlikely. So ignore it for now.
		port->cullDraw(projviewMat, viewportMat, neighborRect, portals, visitedEdgeSet);
	}
	return true;
}

void Portal::setSize(float w, float h){
	width = w;
	height = h;

	struct Vec3 nw(0, 0, h);
	struct Vec3 ne(-w, 0, h);
	struct Vec3 sw(0, 0, 0);
	struct Vec3 se(-w, 0, 0);
	/*nw = getNW();
	ne = getNE();
	sw = getSW();
	se = getSE();*/

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
	setVec3(&nw, &doorPoints[0][2]);
	setVec3(&ne, &doorPoints[0][3]);
	setVec3(&sw, &doorPoints[1][2]);
	setVec3(&se, &doorPoints[1][3]);
	setVec3(&nw, &doorPoints[2][2]);
	setVec3(&sw, &doorPoints[2][3]);
	setVec3(&ne, &doorPoints[3][2]);
	setVec3(&se, &doorPoints[3][3]);
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
	vecAdd(&ret, -width, 0, 0);
	return ret;
}

struct Vec3 Portal::getNW(){
	struct Vec3 ret = pos;
	vecAdd(&ret, 0, 0, height);
	return ret;
}

struct Vec3 Portal::getNE(){
	struct Vec3 ret = pos;
	vecAdd(&ret, -width, 0, height);
	return ret;
}

float Portal::getN()
{
    return pos.z + height;
}

float Portal::getS()
{
    return pos.z;
}

float Portal::getE()
{
    return pos.x - width;
}

float Portal::getW()
{
    return pos.x;
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

bool Portal::in(const struct Vec3* posIn){
	return posIn->x <= pos.x && posIn->x >= pos.x-width && 
			posIn->z >= pos.z && posIn->z <= pos.z+height;
}

MAZEorientation Portal::getOrientation(){
	int verticalVote = (neighbors[0] >= 0) + (neighbors[1] >= 0);
	int horizontalVote = (neighbors[2] >= 0) + (neighbors[3] >= 0);
	if(verticalVote == horizontalVote || (verticalVote == 1 && horizontalVote == 1))
		return JOINT;
	return horizontalVote > verticalVote ? HORIZONTAL : VERTICAL;
}

struct Vec3 Portal::getPos(){
	return pos;
}

void Portal::removeObject(GameObject *obj){
	int nobj = objs.size();
	vector<GameObject *>::iterator iter = objs.begin();
	for(; iter<objs.end(); iter++){
		if(*iter == obj){
			objs.erase(iter);
			break;
		}
	}
}