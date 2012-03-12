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
};

class GameObjectMaker{
public:
	virtual GameObject *make(char *args, MapLoader *mld);
	virtual void parseParam(char *args, GameObjectParam *param);
	void setParam(GameObject *obj, GameObjectParam &param, MapLoader *mld);
};

#endif