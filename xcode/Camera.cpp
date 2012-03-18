//
//  Camera.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Camera.h"
#define VTH 15.0//assuming the threshold of changing the orientation of camera is 15.0 by now

#define LEN 30/25.4
#define ALPHA 20

Camera::Camera() {
	control_m=false;
    pos=Vec3(0.,0.,0.);//the original position need to be changed
	dir=Vec3(0.,0.,1.);
	camO=NORTH;
	ballO=NORTH;
}

const struct Vec3 *Camera::getPos(){
	return &pos;
}

void Camera::updatePos(CamMorientation mov,Keyorientation keyd,Ball *ball,float AngleNS,float AngleEW) 
{
    GLfloat aspectRatio = (GLfloat)800.f/600;
    GLfloat nearClip = 0.1f;
	GLfloat farClip = 4000.0f;
    GLfloat fieldOfView = 90.0f; // TODO Degrees

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
	Vec3 bpos=ball->getPos();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
	//float tmp = sin((ALPHA + AngleNS)/180*PI);
    gluLookAt(bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI), bpos.x, bpos.y, bpos.z, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
	//gluLookAt(bpos.x, bpos.y + 10, bpos.z - 10, bpos.x, bpos.y, bpos.z, 0, 1, 0.);

	if(DEBUG_OUTPUT)
        printf("cam pos %f %f %f\n",pos.x,pos.y,pos.z);
}
