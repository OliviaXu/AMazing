//
//  PhysicsEngine.h
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CS248_Final_Project_PhysicsEngine_h
#define CS248_Final_Project_PhysicsEngine_h

#include <vector>
#include "GameObject.h"
#include "PhysicsInfo.h"
#include "btBulletDynamicsCommon.h"

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    void init();
    void addObject(ObjectType type, PhysicsInfo info);
    void updateObjects(std::vector<GameObject*> &objects);  // ???
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif
