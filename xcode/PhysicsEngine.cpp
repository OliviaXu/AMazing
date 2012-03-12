//
//  PhysicsEngine.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
    
}

PhysicsEngine::~PhysicsEngine() {
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void PhysicsEngine::init() {
    broadphase = new btDbvtBroadphase();
    
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,0,-10));
}

void PhysicsEngine::updateObjects(std::vector<GameObject *> &objects) {
    
}