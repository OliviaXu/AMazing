//
//  Camera.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Camera.h"
# define VTH 15.0//assuming the threshold of changing the orientation of camera is 15.0 by now

#define LEN 50/25.4
#define ALPHA 0
#define PI 3.1415926

Camera::Camera() {
	control_m=false;
	//pos=Vec3(0.,0.+VOFFSET,0.-HOFFSET);//the original position need to be changed
    pos=Vec3(0.,0.,0.);//the original position need to be changed
	dir=Vec3(0.,0.,1.);
	camO=NORTH;
	ballO=NORTH;
}

const struct Vec3 *Camera::getPos(){
	return &pos;
}

void Camera::updatePos(CamMorientation mov,Keyorientation keyd,Ball *ball,float AngleNS,float AngleEW) {
    GLfloat aspectRatio = (GLfloat)800.f/600;
    GLfloat nearClip = 0.1f;
	GLfloat farClip = 4000.0f;
    GLfloat fieldOfView = 120.0f; // TODO Degrees
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
    
	Vec3 bpos=ball->getPos();
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    printf("NS: %f, EW: %f\n", AngleNS, AngleEW);
    
    gluLookAt(bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI), bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI) - sin((AngleNS)/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI) + 10 * cos((AngleNS)/180*PI), -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
    //gluLookAt(bpos.x, bpos.y + LEN * sin(ALPHA), bpos.z - LEN * cos(ALPHA), bpos.x, bpos.y + LEN * sin(ALPHA), bpos.z - LEN * cos(ALPHA) + 1, 0.0, 1.0, 0.0);
    
	if(DEBUG_OUTPUT)
        printf("cam pos %f %f %f\n",pos.x,pos.y,pos.z);
}
