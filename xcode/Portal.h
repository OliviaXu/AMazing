#ifndef CS248_PORTAL_H
#define CS248_PORTAL_H

#include "Framework.h"
#include "Camera.h"
#include "GameObject.h"

typedef struct Rectangle PortalDoor;
enum MAZEorientation{HORIZONTAL, VERTICAL};

class Portal{
public:
    Portal();
    ~Portal();
    void draw(Camera* camera);    // draw all the objects in it
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

private:
	int neighbors[4];
	GameObject *portalObj; //In our case, the corridor
    std::vector<GameObject *> objs;
    std::vector<PortalDoor *> doors;
	struct Vec3 pos; // SW corner
	MAZEmat transformation;
	float width;
	float height;
    //tmp
};

#endif
