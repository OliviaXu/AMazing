//
//  BoxPhysicsInfo.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_BoxPhysicsInfo_h
#define CS248_Final_Project_BoxPhysicsInfo_h

#include "PhysicsInfo.h"

class BoxPhysicsInfo: public PhysicsInfo {
public:
	virtual void parse(char *args);
	virtual PhysicsInfo *clone();
    
    float half_x, half_y, half_z;
};

#endif
