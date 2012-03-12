#ifndef CS248_BALLMAKER_H
#define CS248_BALLMAKER_H

#include "Framework.h"
#include "Ball.h"
#include "GameObjectMaker.h"

class MapLoader;

class BallMaker : public GameObjectMaker{
public:
	BallMaker();
	~BallMaker();
	virtual GameObject *make(char *args, MapLoader *mld);
};

#endif