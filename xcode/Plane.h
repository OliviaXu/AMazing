//
//  Plane.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_Plane_h
#define CS248_Final_Project_Plane_h

#include "Framework.h"

class Plane {
public:
    Plane();
    void update(float dAngleNS, float dAngleEW);
    void getAngle(float &angleNS, float &angleEW);
private:
    float angleNS, angleEW;
};

#endif
