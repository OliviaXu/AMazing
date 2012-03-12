//
//  SpherePhysicsInfo.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_SpherePhysicsInfo_h
#define CS248_Final_Project_SpherePhysicsInfo_h

#include "PhysicsInfo.h"

class SpherePhysicsInfo: public PhysicsInfo {
public:
    SpherePhysicsInfo(float i_trans_x, float i_trans_y, float i_trans_z, float i_trans_w, float i_pos_x, float i_pos_y, float i_pos_z, float i_pos_w, bool i_isStatic, float i_mass, float i_radius): trans_x(i_trans_x), trans_y(i_trans_y), trans_z(i_trans_z), trans_w(i_trans_w), pos_x(i_pos_x), pos_y(i_pos_y), pos_z(i_pos_z), isStatic(i_isStatic), mass(i_mass), radius(i_radius)
    {
        
    }
    float radius;
};

#endif
