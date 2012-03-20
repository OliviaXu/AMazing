//
//  Plane.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Plane.h"

Plane::Plane() {
    this->angleNS=0.;
	this->angleEW=0.;
}

void Plane::update(float dAngleNS, float dAngleEW) {
	this->angleNS=dAngleNS;
	this->angleEW=dAngleEW;
    //this->angleNS+=dAngleNS;
	//this->angleEW+=dAngleEW;
}

void Plane::getAngle(float &angleNS, float &angleEW) {
	angleNS=this->angleNS;
	angleEW=this->angleEW;
    if(DEBUG_OUTPUT)
        printf("test plan angle: %f %f",angleNS,angleEW);
}