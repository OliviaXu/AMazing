//
//  BoxPhysicsInfo.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "BoxPhysicsInfo.h"

void BoxPhysicsInfo::parse(char *args){
	char *str;
	assert(str = strtok(args, " \t"));
	half_x = atoi(str);
    args = str + strlen(str) + 1;
    
	assert(str = strtok(args, " \t"));
	half_y = atoi(str);
    args = str + strlen(str) + 1;
    
	assert(str = strtok(args, " \t"));
	half_z = atoi(str);
    
	PhysicsInfo::shapeTy = PHYSP_PLANE;
	args = str + strlen(str) + 1;
	PhysicsInfo::parse(args);
}

PhysicsInfo *BoxPhysicsInfo::clone(){
	BoxPhysicsInfo *info = new BoxPhysicsInfo();
	info->set(this);
	info->half_x = -half_x / 25.4;
	info->half_y = half_y / 25.4;
	info->half_z = half_z / 25.4;
    
	return info;
}