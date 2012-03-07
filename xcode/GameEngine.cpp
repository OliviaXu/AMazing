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
    
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.FOV, camera.aspectRatio, camera.near, camera.far);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.x + camera.dir.x, camera.pos.y + camera.dir.y, camera.pos.z + camera.dir.z, 0.0, 1.0, 0.0);*/
    
    portal.draw(camera);
}