#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"

typedef struct Rectangle PortalDoor;

class Portal{
public:
	Portal();
	~Portal();
    void draw(Camera &camera);    // draw all the objects in it
    int *getNeighbors();
	void in(struct Vec3 pos);

private:
	int neighbors[4];
	vector<GameObject> objs;
	vector<PortalDoor> doors;
    struct Vec3 pos;    // SW corner
	float width;
	float height;
}

#endif