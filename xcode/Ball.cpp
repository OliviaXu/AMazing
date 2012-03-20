//
//  Ball.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Ball.h"
#include "FunctionalPortal.h"
#include "Hole.h"
#include "OptionBox.h"

Ball::Ball() {
    pos=Vec3(-2.4,0.,4.);
}

float Ball::getV(){
	struct Vec3 v=this->getVelocity();
	return sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));//return the absolute velocity of the ball
}


void Ball::respondToCollision(GameObject *obj, std::queue<MAZEevent> *eq){
	FunctionalPortal *portal = dynamic_cast<FunctionalPortal *>(obj);
	if(portal){
		eq->push(MAZEevent(BALL_INTO_PORTAL, portal));
		return;
	}

	Hole *hole = dynamic_cast<Hole *>(obj);
	if(hole){
		eq->push(MAZEevent(BALL_INTO_HOLE, portal));
		return;
	}

	OptionBox *ob = dynamic_cast<OptionBox *>(obj);
	if(ob){
		switch(ob->ty){
		case OptionBox::START:
			eq->push(MAZEevent(START_GAME, portal));
			return;
		case OptionBox::RESTART:
			eq->push(MAZEevent(RESTART_GAME, portal));
			return;
		case OptionBox::QUIT:
			eq->push(MAZEevent(QUIT_GAME, portal));
			return;
		}
	}
}

float Ball::calcDis(float x, float y, float z)
{
    return (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z);
}