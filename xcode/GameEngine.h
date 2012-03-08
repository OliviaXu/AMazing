//
//  GameEngine.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_GameEngine_h
#define CS248_Final_Project_GameEngine_h

#include "Framework.h"
#include "MapLoader.h"
#include "PhysicsEngine.h"
#include "EventMgr.h"
#include "UserControl.h"
#include "Camera.h"
#include "Plane.h"
#include "Ball.h"

class GameEngine {
public:
    GameEngine(std::string map_file, std::string config_file);    // load map file, load config 
    ~GameEngine();
    void init(sf::Window* window);
    void run();
private:
    void updateObjects();   // update objects if the current portal has changed
    void handleEvents();
    void drawScene();
    
    MapLoader *mapLoader;
    PhysicsEngine *physicsEngine;
    EventMgr *eventMgr;
    UserControl *userControl;
    Camera *camera;
    Plane *plane;
    std::vector<GameObject*> objects;     // objects in the current portal and its neighboring portals
    std::vector<Event> events;   // list of events to be handled
    Ball *ball;
    
    sf::Window *window;
};

#endif
