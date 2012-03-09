//
//  Ball.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Ball.h"

Ball::Ball() {
    pos=Vec3(0.,0.,0.);
}

float Ball::getV(){
	struct Vec3 v=this->getVelocity();
	return sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));//return the absolute velocity of the ball
}