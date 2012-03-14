#ifndef CS248_FUNCTIONALPORTALMAKER_H
#define CS248_FUNCTIONALPORTALMAKER_H

#include "Framework.h"
#include "FunctionalPortal.h"
#include "GameObjectMaker.h"

class MapLoader;

class FunctionalPortalMaker : public GameObjectMaker{
public:
	FunctionalPortalMaker();
	~FunctionalPortalMaker();
	virtual GameObject *make(char *args, MapLoader *mld);

private:
};

#endif