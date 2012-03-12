//
//  PhysicsInfo.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_PhysicsInfo_h
#define CS248_Final_Project_PhysicsInfo_h

enum ObjectType {SPHERE, PLANE, OTHERS};

class PhysicsInfo {
public:
    PhysicsInfo(float i_trans_x, float i_trans_y, float i_trans_z, float i_trans_w, float i_pos_x, float i_pos_y, float i_pos_z, float i_pos_w, bool i_isStatic, float i_mass): trans_x(i_trans_x), trans_y(i_trans_y), trans_z(i_trans_z), trans_w(i_trans_w), pos_x(i_pos_x), pos_y(i_pos_y), pos_z(i_pos_z), isStatic(i_isStatic), mass(i_mass)
    {
    }
    float trans_x, trans_y, trans_z, trans_w;
    float pos_x, pos_y, pos_z;
    bool isStatic;
    float mass;
    
};

#endif
