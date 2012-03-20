#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"
#include "Camera.h"
#include "GameObject.h"

#define MY_FLT_MIN -999999.9
#define MY_FLT_MAX 999999.9

enum MAZEorientation{HORIZONTAL, VERTICAL, JOINT};

class Portal{
public:
    Portal();
    ~Portal();

	//cull or draw this portal. If the portal is drawn, return true;
	//otherwise, false. Basically this function projects portal bounding
	//volumes to windows coordinate and decide whether it shall be culled
    bool cullDraw(struct MAZEmat *projviewMat, struct MAZEmat *viewportMat, 
					struct MAZErectangle &rec, const std::vector<Portal *> *portals, 
						std::set<int *> &visitedEdgeSet);
    int *getNeighbors();
    void addObject(GameObject *obj);
    void setPortalObject(GameObject *obj);
	bool in(const struct Vec3* pos);
	void setSize(float w, float h);
	void setPos(float x, float y, float z);
	struct Vec3 getPos();
	struct Vec3 getSW();
	struct Vec3 getSE();
	struct Vec3 getNW();
	struct Vec3 getNE();
    const float getN();
    const float getS();
    const float getE();
    const float getW();
	struct Vec2 getSize();
	MAZEorientation getOrientation();
	//int doorStatus[4];
	void removeObject(GameObject *obj);

private:
	int neighbors[4];
	
	GameObject *portalObj; //In our case, the corridor
    std::vector<GameObject *> objs;
    //std::vector<PortalDoor *> doors;
	struct Vec3 doorPoints[4][4];
	struct Vec3 pos; // SW corner, y is always 0
	MAZEmat transformation;
	float width;
	float height;
    //tmp
};

#endif
