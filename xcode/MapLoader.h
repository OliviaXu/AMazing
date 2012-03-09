#ifndef CS248_MAPLOADER_H
#define CS248_MAPLOADER_H

#include <string>
#include "Framework.h"
#include "Portal.h"
#include "Shader.h"

// contain all the information of the map

class MapLoader{
public:
	typedef bool (*PortalIterateFun) (Portal *portal, int iPortal, void *auxData);
	MapLoader();
	~MapLoader();
	void load(std::string map_file);
    bool updateCurrentPortal(Vec3 &pos);     // update current portal according to the position of the ball
    void fillObjects(std::vector<GameObject*> &objects);   // fill objects with current portal and its neighboring portals' objects
    Portal *getCurrentPortal();
	int getCurrentPortalIdx();
	void iteratePortals(int rootIdx, PortalIterateFun fun, void *auxData);
	MAZEorientation getCurrentOrientation();

private:
	void readModel();
	void readTexture();
	void loadShader();
	void readPortal();
	void readObject(bool portalObj);
	void assertMapValidity();
	void computePortalPos(Portal *p);

    std::vector<Assimp::Importer*> importers;
	std::vector<const aiScene *> models;
	std::vector<std::vector<unsigned int> *> indexBuff;
	std::vector<sf::Image *> textures;
    std::vector<Portal *> portals;
	std::vector<GameObject *> objs;
	std::vector<Shader *> shaders;
	bool *visitBuff;//For portal iteration. visitBuff[i] = true means the ith portal has been
					//visited
    int currentPortal;
	MAZEorientation currentOrient;
};

#endif