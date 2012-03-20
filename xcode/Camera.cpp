//
//  Camera.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Camera.h"
#define VTH 15.0//assuming the threshold of changing the orientation of camera is 15.0 by now

#define LEN 150/25.4
#define ALPHA 90
#define HDIS 5
#define VDIS 3

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
	Vec3 bpos=*(ball->getPos());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    //gluLookAt(bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI), bpos.x, bpos.y, bpos.z, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
    //gluLookAt(bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI), bpos.x, bpos.y + LEN * sin((ALPHA + AngleNS)/180*PI) - sin(AngleNS/180*PI), bpos.z - LEN * cos((ALPHA + AngleNS)/180*PI) + cos(AngleNS/180*PI), -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
    //float eye_y = bpos.y + HDIS;
    //float eye_z = bpos.z - VDIS;
    
    float cost = cos(AngleNS/180*PI);
    float sint = sin(AngleNS/180*PI);
    float eye_y = bpos.y + VDIS * cost;
    float eye_dif = HDIS / cost + VDIS * sint;
    switch (keyd) {
        case UP:
            gluLookAt(bpos.x, eye_y, bpos.z - eye_dif, bpos.x, eye_y - sint, bpos.z - eye_dif + cost, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
            break;
        case DOWN:
            gluLookAt(bpos.x, eye_y, bpos.z + eye_dif, bpos.x, eye_y - sint, bpos.z + eye_dif + cost, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
            break;
        case LEFT:
            gluLookAt(bpos.x - eye_dif, eye_y, bpos.z, bpos.x - eye_dif + cost, eye_y - sint, bpos.z, 0., cos((AngleEW)/180*PI), -sin((AngleEW)/180*PI));
            break;
        case RIGHT:
            gluLookAt(bpos.x + eye_dif, eye_y, bpos.z, bpos.x + eye_dif + cost, eye_y - sint, bpos.z, 0., cos((AngleEW)/180*PI), sin((AngleEW)/180*PI));
            break;
    }
    
	//pos.x = bpos.x;
	//pos.y = eye_y;
	//pos.z = eye_z;

	if(DEBUG_OUTPUT)
        printf("cam pos %f %f %f\n",pos.x,pos.y,pos.z);
}
