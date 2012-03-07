//
//  GameEngine.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "GameEngine.h"

GameEngine::GameEngine(string map_file, string config_file)
{
    mapLoader = new MapLoader();
    physicsEngine = new PhysicsEngine();
    objects = new vector<GameEngine>();
    eventMgr = new EventMgr();
    userControl = new UserControl();
    camera = new Camera();
    plane = new Plane();
    mapLoader.load(map_file);
    physicsEngine.init();
    ball = new Ball();
}

GameEngine::~GameEngine()
{
    
}

void init(sf::Window window)
{
    this->window = window;
    userControl.setWindow(window);
}

void GameEngine::run()
{
    while(1){
        userControl.handleInput(window);    // constant * window.GetFrameTime() 
        
        float dAngleNS, dAngleEW;
        userControl.getAngleUpdate(dAngleNS, dAngleEW);
        
        plane.update(dAngleNS, dAngleEW);
        
        if(mapLoader.updateCurrentPortal(ball.getPos()))
            updateObjects();
        
        physicsEngine.updateObjects(objects);
        
        eventMgr.updateEvents(objects, mapLoader, events);
        handleEvents();
        
        camera.updatePos(ball.getPos());
        
        drawScene();
        
        window.display();
    }
}

void GameEngine::updateObjects()
{
    objects.clear();
    mapLoader.fillObjects(objects);
}

void GameEngine::handleEvents()
{
    
}

void GameEngine::drawScene()
{
    Portal portal = mapLoader.getCurrentPortal();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], eyePos[0] + lookAtX[face], eyePos[1] + lookAtY[face], eyePos[2] + lookAtZ[face], upX[face], upY[face], upZ[face]);
}