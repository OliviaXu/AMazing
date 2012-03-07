#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"

typedef struct Rectangle PortalDoor;

class Portal{
public:
	Portal();
	~Portal();
who are you?
private:
	int neighbors[4];
	vector<GameObject> objs;
	vector<PortalDoor> doors;
	float width;
	float height;
}

#endif
