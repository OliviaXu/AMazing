//
//  Ball.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_Ball_h
#define CS248_Final_Project_Ball_h

#include "GameObject.h"

class Ball: public GameObject {
	//test
public:
    Ball();
	float getV();
    float calcDis(float x, float y, float z);
	virtual void respondToCollision(GameObject *obj, std::queue<MAZEevent> *eq);
};

#endif
