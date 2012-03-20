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

#define FLOOR_DIS 6.0
#define CAM_HEIGHT 4.0
#define FOLLOW_RATE 0.1
#define LOOKAT_BALL 0

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
    
    switch(keyd)
    {
        case UP:
            AngleNS = -AngleNS;
            break;
        case DOWN:
            AngleNS = AngleNS;
            break;
        case LEFT:
        {
            float tmp = AngleEW;
            AngleEW = -AngleNS; // should be AngleNS, don't know why their is a minus sign
            AngleNS = tmp;
            break;
        }
        case RIGHT:
        {
            float tmp = AngleEW;
            AngleEW = -AngleNS; // should be AngleNS, don't know why their is a minus sign
            AngleNS = -tmp;
            break;
        }
    }
    
    float cam_lift = CAM_HEIGHT * cos(AngleNS/180*PI);
    float cam_back = FLOOR_DIS + CAM_HEIGHT * sin(AngleNS/180*PI);
    float ball_down_look_angle = atan(cam_lift/cam_back);
#ifdef LOOKAT_BALL
    float final_look_angle = AngleNS/180*PI - FOLLOW_RATE * (AngleNS/180*PI + ball_down_look_angle);
#else
    float final_look_angle = (1 - FOLLOW_RATE) * AngleNS;
#endif
    float look_y_dif = sin(final_look_angle);
    float look_z_dif = cos(final_look_angle);
    
    switch (keyd) {
        case UP:
            //gluLookAt(bpos.x, eye_y, bpos.z - eye_dif, bpos.x, eye_y - sint, bpos.z - eye_dif + cost, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
            pos.x = bpos.x;
            pos.y = bpos.y;
            pos.z = bpos.z - cam_back;
            gluLookAt(bpos.x, bpos.y + cam_lift, bpos.z - cam_back, bpos.x, bpos.y + cam_lift + look_y_dif, bpos.z - cam_back + look_z_dif, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
            break;
        case DOWN:
            pos.x = bpos.x;
            pos.y = bpos.y;
            pos.z = bpos.z + cam_back;
            gluLookAt(bpos.x, bpos.y + cam_lift, bpos.z + cam_back, bpos.x, bpos.y + cam_lift + look_y_dif, bpos.z + cam_back - look_z_dif, -sin((AngleEW)/180*PI), cos((AngleEW)/180*PI), 0.);
            break;
        case LEFT:
            pos.x = bpos.x - cam_back;
            pos.y = bpos.y;
            pos.z = bpos.z;
            gluLookAt(bpos.x - cam_back, bpos.y + cam_lift, bpos.z, bpos.x - cam_back + look_z_dif, bpos.y + cam_lift + look_y_dif, bpos.z, 0., cos((AngleEW)/180*PI), -sin((AngleEW)/180*PI));
            break;
        case RIGHT:
            pos.x = bpos.x + cam_back;
            pos.y = bpos.y;
            pos.z = bpos.z;
            gluLookAt(bpos.x + cam_back, bpos.y + cam_lift, bpos.z, bpos.x + cam_back - look_z_dif, bpos.y + cam_lift + look_y_dif, bpos.z, 0., cos((AngleEW)/180*PI), -sin((AngleEW)/180*PI));
            break;
    }
    
	//pos.x = bpos.x;
	//pos.y = eye_y;
	//pos.z = eye_z;

	if(DEBUG_OUTPUT)
        printf("cam pos %f %f %f\n",pos.x,pos.y,pos.z);
}
