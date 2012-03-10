#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"
#include "Camera.h"
#include "GameObject.h"

enum MAZEorientation{HORIZONTAL, VERTICAL, JOINT};

class Portal{
public:
    Portal();
    ~Portal();

	//cull or draw this portal. If the portal is drawn, return true;
	//otherwise, false. Basically this function projects portal bounding
	//volumes to windows coordinate and decide whether it shall be culled
    bool cullDraw(struct MAZEmat *projMat, struct MAZErectangle &rec, std::vector<Portal *> *portals);
    int *getNeighbors();
    void addObject(GameObject *obj);
    void setPortalObject(GameObject *obj);
	bool in(struct Vec3* pos);
	void setSize(float w, float h);
	void setPos(float x, float y, float z);
	struct Vec3 getSW();
	struct Vec3 getSE();
	struct Vec3 getNW();
	struct Vec3 getNE();
	struct Vec2 getSize();
	MAZEorientation getOrientation();
	int doorStatus[4];

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
