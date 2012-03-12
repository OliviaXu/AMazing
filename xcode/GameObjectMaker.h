#ifndef CS248_GAMEOBJECTMAKER_H
#define CS248_GAMEOBJECTMAKER_H

#include "Framework.h"
#include "GameObject.h"

class MapLoader;
struct GameObjectParam{
	int iPortal;
	int iShader;
	int iMesh;
	int x, y, z;
	int mass;
	int iDTex;
	int iSTex;
	int iPhyInfo;
};

class GameObjectMaker{
public:
	virtual GameObject *make(char *args, MapLoader *mld);
	virtual void parseParam(char *args, struct GameObjectParam *param);
	void setParam(GameObject *obj, struct GameObjectParam &param, MapLoader *mld);
};

#endif