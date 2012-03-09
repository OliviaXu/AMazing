//
//  Camera.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Camera.h"

Camera::Camera() {
    
}

void Camera::updatePos(struct Vec3 &newBallPos) {
    //Just some shameful hack to render the scene. MUST replace this with mature code!

    GLfloat aspectRatio = (GLfloat)800.f/600;
    GLfloat nearClip = 0.1f;
	GLfloat farClip = 400.0f;
    GLfloat fieldOfView = 45.0f; // Degrees

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//computer camera location according to the location of the ball

	//need to be set according to the orientation of the portal and previous moving direction...
	//get current portal, width>height
	struct Vec3 camloc(newBallPos.x,newBallPos.y+50,newBallPos.z+50);//about the z+- where...
	//get the direction according to ...
	//only can be either (+-1.,0.,0.) or (0.,0.,+-1.)
	//how did we solve the camera direction? how did we computer...
	//gluLookAt(camloc.x,camloc.y,camloc.z,dir.x,dir.y,dir.z,0.,1.,0.);
	gluLookAt(20, 0, 0, 0, 0, 0, 0, 1, 0);

}
