#include "Makers.h"
#include "MapLoader.h"
#include "Ball.h"
#include "Wall.h"
#include "FunctionalPortal.h"

/*------------------------------------------------------------------------------
 *				GameObjectMaker
 *------------------------------------------------------------------------------*/
GameObject *GameObjectMaker::make(char *args, MapLoader *mld){
	
	struct GameObjectParam param;
	parseParam(args, &param);
	GameObject *obj = new GameObject();

	setParam(obj, param, mld);
	return obj;
}

void GameObjectMaker::setParam(GameObject *obj, struct GameObjectParam &param, MapLoader *mld){
	//assert(portals.size() > param.iPortal);
	obj->setPortal(param.iPortal);

	obj->setMass(param.mass);

    Vec3 pos = Vec3(-param.x/25.4, param.y/25.4, param.z/25.4);
	Portal *portal = (Portal *)mld->getPortal(param.iPortal);
	Vec3 worldPos = portal->getPos();
	worldPos.x = pos.x + worldPos.x;
	worldPos.y = pos.y + worldPos.y;
	worldPos.z = pos.z + worldPos.z;

	obj->setPos(worldPos);

	//assert(shaders.size() > prop->iShader);
	obj->setShader(mld->getShader(param.iShader));

	//assert(models.size() > prop->iMesh);
	obj->setModel(mld->getModel(param.iMesh), mld->getIndexBuff(param.iMesh));//Here we make the
											//assumption that each model contains only
											//one mesh
	//std::vector<sf::Image *> hey = textures;
	//assert((int)(textures.size()) > prop->iDTex && (int)(textures.size()) > prop->iSTex);
	const sf::Image *dtex = NULL;
	const sf::Image *stex = NULL;
	if(param.iDTex >= 0)
		dtex = mld->getTexture(param.iDTex);
	if(param.iSTex >= 0)
		stex = mld->getTexture(param.iSTex);
	obj->setTexture(dtex, stex);

	obj->phyinfo = mld->phyinfos[param.iPhyInfo]->clone();

	pos = obj->getPos();
	obj->phyinfo->pos_x = pos.x - obj->phyinfo->pos_x;
	obj->phyinfo->pos_y = pos.y + obj->phyinfo->pos_y;
	obj->phyinfo->pos_z = pos.z + obj->phyinfo->pos_z;
}


void GameObjectMaker::parseParam(char *args, GameObjectParam *param){
	char *str;

	assert(str = strtok(args, " \t"));
	param->iMesh = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->iPortal = atoi(str);
	
	assert(str = strtok(NULL, " \t"));
	param->x = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->y = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->z = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->iShader = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->iDTex = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->iSTex = atoi(str);

	assert(str = strtok(NULL, " \t"));
	param->iPhyInfo = atoi(str);
}

/*------------------------------------------------------------------------------
 *				BallMaker
 *------------------------------------------------------------------------------*/
BallMaker::BallMaker(){

}

BallMaker::~BallMaker(){

}

GameObject *BallMaker::make(char *args, MapLoader *mld){
	struct GameObjectParam param;
	parseParam(args, &param);
	Ball *ball = new Ball();

	setParam(ball, param, mld);
	return ball;
}

/*------------------------------------------------------------------------------
 *				FunctionalPortalMaker
 *------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
 *				WallMaker
 *------------------------------------------------------------------------------*/
WallMaker::WallMaker(){

}

WallMaker::~WallMaker(){

}

GameObject *WallMaker::make(char *args, MapLoader *mld){
	struct GameObjectParam param;
	parseParam(args, &param);
	Wall *wall = new Wall();
	
	setParam(wall, param, mld);
	return wall;
}