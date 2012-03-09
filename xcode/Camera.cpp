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
	gluLookAt(100, 0, 0, 0, 0, 0, 0, 1, 0);
}