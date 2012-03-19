#ifndef CS248_MAPLOADER_H
#define CS248_MAPLOADER_H

#include <string>
#include "Framework.h"
#include "Portal.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include "PhysicsInfo.h"
#include "PhysicsEngine.h"
#include "Ball.h"

// contain all the information of the map

enum MAZEportal_cull{CULL_NEIGHBORS, CULL_REST, CULL_NONE};
typedef MAZEportal_cull (*PortalIterateFun) (Portal *portal, int iPortal, void *auxData);

class MapLoader{
public:
	MapLoader();
	~MapLoader();
	void load(std::string map_file, PhysicsEngine *engine);
    bool updateCurrentPortal(const struct Vec3 *pos);     // update current portal according to the position of the ball
    void fillObjects(std::vector<GameObject*> &objects);   // fill objects with current portal and its neighboring portals' objects
	int getCurrentPortalIdx();
	void iteratePortals(int rootIdx, PortalIterateFun fun, void *auxData);
	MAZEorientation getCurrentOrientation();
	
	const Portal *getCurrentPortal();
	const std::vector<Portal *> *getPortals();
	const aiScene *getModel(int iModel);
	const std::vector<unsigned int> *getIndexBuff(int iBuff);
	const sf::Image *getTexture(int iTex);
	const Portal *getPortal(int iPortal);
	const Shader *getShader(int iShader);
    void updateObjPortal(GameObject *obj);
    std::vector<GameObject *>* getObject();
	
	std::vector<PhysicsInfo *> phyinfos;
	Ball *getBall();

private:
	void readModel();
	void readTexture();
	void loadShader();
	void readPortal();
	void readPhyInfo();
	void readObject(bool portalObj, PhysicsEngine *engine);
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
	GameObjectFactory factory;
	Ball *ball;
};

#endif
