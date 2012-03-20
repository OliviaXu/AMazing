#ifndef CS248_MAKERS_H
#define CS248_MAKERS_H

#include "Framework.h"
#include "Ball.h"

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
	virtual char *parseParam(char *args, struct GameObjectParam *param);
	void setParam(GameObject *obj, struct GameObjectParam &param, MapLoader *mld);
};

class BallMaker : public GameObjectMaker{
public:
	BallMaker();
	~BallMaker();
	virtual GameObject *make(char *args, MapLoader *mld);
};

class FunctionalPortalMaker : public GameObjectMaker{
public:
	FunctionalPortalMaker();
	~FunctionalPortalMaker();
	virtual GameObject *make(char *args, MapLoader *mld);

private:
};

class WallMaker : public GameObjectMaker{
public:
	WallMaker();
	~WallMaker();
	virtual GameObject *make(char *args, MapLoader *mld);
};

class HoleMaker : public GameObjectMaker{
public:
	HoleMaker();
	~HoleMaker();
	virtual GameObject *make(char *args, MapLoader *mld);
};

class OptionBoxMaker : public GameObjectMaker{
public:
	OptionBoxMaker();
	~OptionBoxMaker();
	virtual GameObject *make(char *args, MapLoader *mld);
};

#endif