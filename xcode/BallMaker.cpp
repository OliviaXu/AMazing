#include "BallMaker.h"

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