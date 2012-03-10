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
    userControl->setWindow(window);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
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

void GameEngine::drawScene()
{
    Portal *portal = mapLoader->getCurrentPortal();
    
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.FOV, camera.aspectRatio, camera.near, camera.far);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.x + camera.dir.x, camera.pos.y + camera.dir.y, camera.pos.z + camera.dir.z, 0.0, 1.0, 0.0);*/
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0; i<mapLoader->portals.size(); i++)
		mapLoader->portals[i]->draw(camera);
}