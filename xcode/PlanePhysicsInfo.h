//
//  PlanePhysicsInfo.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_PlanePhysicsInfo_h
#define CS248_Final_Project_PlanePhysicsInfo_h

#include "PhysicsInfo.h"

class PlanePhysicsInfo: public PhysicsInfo {
public:
	virtual void parse(char *args);
	virtual PhysicsInfo *clone();

    float normal_x, normal_y, normal_z;
};

#endif
