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
    PlanePhysicsInfo(float i_trans_x, float i_trans_y, float i_trans_z, float i_trans_w, float i_pos_x, float i_pos_y, float i_pos_z, float i_pos_w, bool i_isStatic, float i_mass, float i_normal_x, float i_normal_y, float i_normal_z): trans_x(i_trans_x), trans_y(i_trans_y), trans_z(i_trans_z), trans_w(i_trans_w), pos_x(i_pos_x), pos_y(i_pos_y), pos_z(i_pos_z), isStatic(i_isStatic), mass(i_mass), normal_x(i_normal_x), normal_y(i_normal_y), normal_z(i_normal_z)
    {
        
    }
    float normal_x, normal_y, normal_z;
};

#endif
