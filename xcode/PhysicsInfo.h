//
//  PhysicsInfo.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_PhysicsInfo_h
#define CS248_Final_Project_PhysicsInfo_h

#include "Framework.h"

enum PhysicsShapeTy{PHYSP_PLANE, PHYSP_SPHERE, PHYSP_BOX, PHYSP_OTHERS};

class PhysicsInfo {
public:
    virtual void parse(char *args);
	virtual PhysicsInfo *clone() = 0;
	virtual void set(PhysicsInfo *info);

    float trans_x, trans_y, trans_z, trans_w;
    float pos_x, pos_y, pos_z;
    int is_static;
    float mass;
    PhysicsShapeTy shapeTy;
	int forceActivation;
};

#endif
