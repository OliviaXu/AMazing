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
	pos=Vec3(0.,0.+VOFFSET,0.-HOFFSET);//the original position need to be changed
	dir=Vec3(0.,0.,1.);
	camO=NORTH;
	ballO=NORTH;
}

const struct Vec3 *Camera::getPos(){
	return &pos;
}

void Camera::updatePos(CamMorientation mov,Keyorientation keyd,Ball *ball) {
    //Just some shameful hack to render the scene. MUST replace this with matu code!

    GLfloat aspectRatio = (GLfloat)800.f/600;
    GLfloat nearClip = 0.1f;
	GLfloat farClip = 4000.0f;
    GLfloat fieldOfView = 90.0f; // TODO Degrees

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3 ballv=ball->getVelocity();
	//TODO:what if all 0?
	//still?
	
	if(abs(ballv.x)>abs(ballv.z)){
		if(ballv.x>0){
			ballO=EAST;
		}else if(ballv.x<0){
			ballO=WEST;
		}
	}else{
		if(ballv.z>0){
			ballO=NORTH;
		}else if(ballv.z<0){
			ballO=SOUTH;
		}
	}

	switch (ballO){
      case EAST:
		  dir=Vec3(1.,0.,0.);
		break;
	  case WEST:
		  dir=Vec3(-1.,0.,0.);
		  break;
	  case SOUTH:
		  dir=Vec3(0.,0.,-1.);
		  break;
	  case NORTH:
		  dir=Vec3(0.,0.,1.);
		  break;
	}
	//to get the direction of offset
	Vec3 dircpy(dir.x,dir.y,dir.z);
	VMulti(&dircpy,-1*HOFFSET);
	//offset the camera
	Vec3 bpos=ball->getPos();

	//need to be enable when the ball rolls
	//pos=vec2vecAdd(&bpos,dircpy.x,VOFFSET,dircpy.z);
	
	//vecAdd(&pos,dircpy.x,VOFFSET,dircpy.z);
	//printf("ball p %f %f %f\n",ball->getPos().x,ball->getPos().y,ball->getPos().z);
	//printf("position %f %f %f\n",pos.x,pos.y,pos.z);
	//get exact camera direction according to the camera
	camO=(Morientation)((ballO+keyd)%4);
	//printf("camear direction %d .",camO);
	//printf("ball   direction %d .",ballO);
	if(keyd==0){//directly looking forward no change to the camera direction//camera direction purely depends on the ball direction
	}else{
		switch (camO){
		  case EAST:
			  dir=Vec3(1.,0.,0.);
			break;
		  case WEST:
			  dir=Vec3(-1.,0.,0.);
			  break;
		  case SOUTH:
			  dir=Vec3(0.,0.,-1.);
			  break;
		  case NORTH:
			  dir=Vec3(0.,0.,1.);
			  break;
		}
	}
	//now the camera is purely follows the ball
	/*
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
	
	*/
	switch(mov){
	case U:
		pos.y++;
		break;
	case D:
		pos.y--;
		break;
	case L:
		pos.x++;
		break;
	case R:
		pos.x--;
		break;
	case F:
		pos.z++;
		break;
	case B:
		pos.z--;
	}
	gluLookAt(pos.x,pos.y,pos.z,pos.x+dir.x,pos.y+dir.y,pos.z+dir.z,0.,1.,0.);
	printf("cam pos %f %f %f\n",pos.x,pos.y,pos.z);
	//gluLookAt(100, 0, 0, 0, 0, 0, 0, 1, 0);
}
