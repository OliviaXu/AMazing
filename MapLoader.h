#ifndef CS248_MAPLOADER_H
#define CS248_MAPLOADER_H

#include <string.h>
#include "Framework.h"

// contain all the information of the map

class MapLoader{
public:
	MapLoader();
	~MapLoader();
	void load(string map_file);
    bool updateCurrentPortal(Vec3 pos);     // update current portal according to the position of the ball
    void fillObjects(vector<GameObject> &objects);   // fill objects with current portal and its neighboring portals' objects
    Portal getCurrentPortal();
	
private:
    vector<Portal> portals;
    int currentPortal;
};

#endif