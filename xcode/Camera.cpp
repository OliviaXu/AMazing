//
//  Camera.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Camera.h"
# define VTH 15.0//assuming the threshold of changing the orientation of camera is 15.0 by now

Camera::Camera() {
	pos=Vec3(0.,0.,0.);//the original position need to be changed
	dir=Vec3(0.,0.,1.);
    
}

void Camera::updatePos(MAZEorientation pOri,Ball *ball) {
    //Just some shameful hack to render the scene. MUST replace this with matu code!

    GLfloat aspectRatio = (GLfloat)800.f/600;
    GLfloat nearClip = 0.1f;
	GLfloat farClip = 400.0f;
    GLfloat fieldOfView = 45.0f; // Degrees

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3 ballv=ball->getVelocity();
	//is the any delay about transition?
	//first need to see which is the direction if x>y...
	if(ball->getV()>VTH){
		//change the direction according to the direction of the ball
		if(Vequal(&dir,&(Vec3(1.,0.,0.)))||Vequal(&dir,&(Vec3(-1.,0.,0.)))){
			//if originally the direction is ...
			VMulti(&dir,ballv.x/abs(ballv.x));
		}
		
	}

	if(pOri==HORIZONTAL){
		//(+-1.,0.,0.) 1 according to the previous direction...
		dir=Vec3(1.,0.,0.);//assuming the maze is from lower left to upper right... therefore everytime when enter a portal it is always?... no backwards...
	}
	else if(pOri==VERTICAL){//VERTICAL
		dir=Vec3(0.,0.,1.)
	}else if(pOri==JOINT){
	}

	//compute camera location according to the location of the ball

	//need to be set according to the orientation of the portal and previous moving direction...
	//get current portal, width>height
	//pos=ball->getPos()+;
	//actually the direction should be lookat with some offset ...(one is from y axis ... the other one is from the direction of camera[not current directionof ball...])
	//struct Vec3 camloc(newBallPos.x,newBallPos.y+50.,newBallPos.z+50.);//about the z+- where...
	//get the direction according to ...
	//only can be either (+-1.,0.,0.) or (0.,0.,+-1.)
	//how did we solve the camera direction? how did we computer...
	//according to the speed of the ball if it is larget than a cetain speed in one direction then ..
	//therefore for camera need current direction... then compute the ball v.. see whether we need to change the direction according to the current ball location
	//gluLookAt(pos.x,pos.y,pos.z,pos.x+dir.x,pos.y+dir.y,pos.z+dir.z,0.,1.,0.);
	gluLookAt(100, 0, 0, 0, 0, 0, 0, 1, 0);

}
