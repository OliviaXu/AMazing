#include "Makers.h"
#include "MapLoader.h"
#include "Ball.h"
#include "Wall.h"
#include "FunctionalPortal.h"
#include "Hole.h"
#include "OptionBox.h"

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
	obj->phyinfo->forceActivation = 0;
}


char *GameObjectMaker::parseParam(char *args, GameObjectParam *param){
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

	return str + strlen(str) + 1;
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

	ball->phyinfo->forceActivation = 1;
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
	args = parseParam(args, &param);
	FunctionalPortal *fp = new FunctionalPortal();
	char *str;
	
	assert(str = strtok(args, " \t"));
	int iDestPortal = atoi(str);
	assert(str = strtok(NULL, " \t"));
	int dir = atoi(str);

	assert(str = strtok(NULL, " \t"));
	float dest_x = -atoi(str) / 25.4;
	assert(str = strtok(NULL, " \t"));
	float dest_y = atoi(str) / 25.4;
	assert(str = strtok(NULL, " \t"));
	float dest_z = atoi(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	int src_lookDir = atoi(str);

		std::cout << iDestPortal << dir << src_lookDir << std::endl;
	/*assert(str = strtok(args, " \t"));
	float look_x = atoi(str);
	assert(str = strtok(args, " \t"));
	float look_y = atoi(str);
	assert(str = strtok(args, " \t"));
	float look_z = atoi(str);*/

	setParam(fp, param, mld);

	fp->setPosDir(&(struct Vec3(dest_x, dest_y, dest_z)), dir, iDestPortal, src_lookDir);
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
	args = parseParam(args, &param);

	char *str;
	assert(str = strtok(args, " \t"));
	int iNormalTex = atoi(str);

	assert(str = strtok(args, " \t"));
	int iDepthTex = atoi(str);
	
	Wall *wall = new Wall();
	wall->setNormalTex(mld->getTexture(iNormalTex));
	wall->setDepthTex(mld->getTexture(iDepthTex));

	setParam(wall, param, mld);
	
	return wall;
}

/*------------------------------------------------------------------------------
 *				HoleMaker
 *------------------------------------------------------------------------------*/
HoleMaker::HoleMaker(){

}

HoleMaker::~HoleMaker(){

}

GameObject *HoleMaker::make(char *args, MapLoader *mld){
	struct GameObjectParam param;
	args = parseParam(args, &param);

	Hole *hole = new Hole();

	setParam(hole, param, mld);
	
	return hole;
}

/*------------------------------------------------------------------------------
 *				OptionBoxMaker
 *------------------------------------------------------------------------------*/
OptionBoxMaker::OptionBoxMaker(){

}

OptionBoxMaker::~OptionBoxMaker(){

}

GameObject *OptionBoxMaker::make(char *args, MapLoader *mld){
	struct GameObjectParam param;
	args = parseParam(args, &param);

	char *str;
	assert(str = strtok(args, " \t"));
	OptionBox *ob = new OptionBox();
	if(strcmp(str, "start") == 0)
		ob->ty = OptionBox::START;
	else if(strcmp(str, "quit") == 0)
		ob->ty = OptionBox::QUIT;
	else
		ob->ty = OptionBox::RESTART;
	setParam(ob, param, mld);
	
	return ob;
}