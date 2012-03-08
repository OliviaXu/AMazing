#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"
#include "Camera.h"
#include "GameObject.h"

typedef struct Rectangle PortalDoor;

class Portal{
public:
    Portal();
    ~Portal();
    void draw(Camera* camera);    // draw all the objects in it
    int *getNeighbors();
    void addObject(GameObject *obj);
    void setPortalObject(GameObject *obj);
	bool in(struct Vec3* pos);

private:
	int neighbors[4];
	GameObject *portalObj; //In our case, the corridor
    std::vector<GameObject *> objs;
    std::vector<PortalDoor *> doors;
	struct Vec3 pos; // SW corner
	float width;
	float height;
};

#endif
