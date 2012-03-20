#include "GameObjectFactory.h"
#include "Makers.h"
#include "MapLoader.h"

using namespace std;

GameObjectFactory::GameObjectFactory(){
	makers["gameobject"] = new GameObjectMaker();
	makers["ball"] = new BallMaker();
	makers["funportal"] = new FunctionalPortalMaker();
	makers["wall"] = new WallMaker();
	makers["optionbox"] = new OptionBoxMaker();
	makers["hole"] = new HoleMaker();
	makers["wallfunportal"] = new WallFunctionalPortalMaker();
}

GameObjectFactory::~GameObjectFactory(){
	map<string, GameObjectMaker *>::iterator iter = makers.begin();
	while(true){
		delete (*iter).second;
		if(iter == makers.end())
			break;
		iter++;
	}
}

GameObject *GameObjectFactory::produce(string obj_name, char *args, MapLoader *mld){
	assert(makers.count(obj_name));

	return makers[obj_name]->make(args, mld);
}