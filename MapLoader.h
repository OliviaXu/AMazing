#ifndef CS248_MAPLOADER_H
#define CS248_MAPLOADER_H

#include <string>
#include "Framework.h"
#include "Portal.h"

// contain all the information of the map

class MapLoader{
public:
	typedef bool (*PortalIterateFun) (Portal *portal, void *auxData);
	MapLoader();
	~MapLoader();
	void load(std::string map_file);
    bool updateCurrentPortal(Vec3 pos);     // update current portal according to the position of the ball
    void fillObjects(std::vector<GameObject> &objects);   // fill objects with current portal and its neighboring portals' objects
    Portal *getCurrentPortal();
	void iteratePortals(int rootIdx, PortalIterateFun fun, void *auxData);

private:
    std::vector<Portal *> portals;
	bool *visitBuff;
    int currentPortal;
};

#endif