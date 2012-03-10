//
//  GameEngine.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "GameEngine.h"
using namespace std;

GameEngine::GameEngine(string map_file, string config_file)
{
    mapLoader = new MapLoader();
    physicsEngine = new PhysicsEngine();
    eventMgr = new EventMgr();
    userControl = new UserControl();
    camera = new Camera();
    plane = new Plane();
    mapLoader->load(map_file);
    physicsEngine->init();
    ball = new Ball();
}

GameEngine::~GameEngine()
{
    delete mapLoader;
	delete physicsEngine;//Do we need further clean up?
	delete eventMgr;
	delete userControl;
	delete camera;
	delete plane;
	delete ball;
}

void GameEngine::init(sf::Window* _window)
{
    this->window = _window;
	viewport.bottom = 0;
	viewport.left = 0;
	viewport.width = window->GetWidth();
	viewport.height = window->GetHeight();

    userControl->setWindow(window);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

}

void GameEngine::run()
{
    while(1){
        userControl->handleInput();    // constant * window.GetFrameTime() 

		//what's the reason for this??? 
        
        float dAngleNS, dAngleEW;
        userControl->getAngleUpdate(dAngleNS, dAngleEW);
        
        plane->update(dAngleNS, dAngleEW);
        
        if(mapLoader->updateCurrentPortal(ball->getPos()))
            updateObjects();
        
        physicsEngine->updateObjects(objects);
        
        eventMgr->updateEvents(objects, mapLoader, events);
        handleEvents();
        
		camera->updatePos(userControl->getCamM(),userControl->getCamDirUpdate(),ball);//input camera movement ball direction and ball to determin camera position and direction

		struct Vec3 lP=userControl->lightPos();
		GLfloat light_position[]={lP.x,lP.y,lP.z,0.};//directinal light;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		printf("cam mov: % d light: %f %f %f\n",userControl->getCamM(),light_position[0],light_position[1],light_position[2]);
        drawScene();
        
        window->Display();
    }
}

void GameEngine::updateObjects()
{
    objects.clear();
    mapLoader->fillObjects(objects);
}

void GameEngine::handleEvents()
{
    
}

void getWindowProjMat(struct MAZErectangle &viewport, struct MAZEmat *winProjMat){
	MAZEmat modelviewMat;
	MAZEmat projMat;
	MAZEmat viewportMat;
	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMat.mat);
	glGetFloatv(GL_PROJECTION_MATRIX, projMat.mat);
	
	float farv = 1., nearv = 0.;
	viewportMat.mat[0] = viewport.width/2;
	viewportMat.mat[5] = viewport.height/2;
	viewportMat.mat[10] = (farv-nearv)/2;
	viewportMat.mat[12] = viewport.width/2;
	viewportMat.mat[13] = viewport.height/2;
	viewportMat.mat[14] = (farv + nearv)/2;

	MAZEmat tmp;
	multMat(&viewportMat, &projMat, &tmp);
	multMat(&tmp, &modelviewMat, winProjMat);
}

void GameEngine::drawScene()
{
    Portal *portal = mapLoader->getCurrentPortal();
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int iRootPortal = mapLoader->getCurrentPortalIdx();

	struct MAZEmat projMat;
	
	getWindowProjMat(viewport, &projMat);

	Portal *rootPortal = mapLoader->getCurrentPortal();
	rootPortal->doorStatus[0] = 0;
	rootPortal->doorStatus[1] = 0;
	rootPortal->doorStatus[2] = 0;
	rootPortal->doorStatus[3] = 0;

	rootPortal->cullDraw(&projMat, viewport, mapLoader->getPortals());
}
