#ifndef CS248_GAMEOBJECTFACTORY_H
#define CS248_GAMEOBJECTFACTORY_H

#include "Framework.h"
#include "GameObjectMaker.h"

class MapLoader;

class GameObjectFactory{
public:
	GameObjectFactory();
	~GameObjectFactory();
	GameObject *produce(std::string obj_name, char *args, MapLoader *mld);

private:
	std::map<std::string, GameObjectMaker *> makers;
};

#endif