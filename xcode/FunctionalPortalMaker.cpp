#include "FunctionalPortalMaker.h"

FunctionalPortalMaker::FunctionalPortalMaker(){

}

FunctionalPortalMaker::~FunctionalPortalMaker(){

}

GameObject *FunctionalPortalMaker::make(char *args, MapLoader *mld){
	struct GameObjectParam param;
	parseParam(args, &param);
	FunctionalPortal *fp = new FunctionalPortal();

	setParam(fp, param, mld);
	return fp;
}