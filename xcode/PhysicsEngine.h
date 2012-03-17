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
    void addObject(PhysicsShapeTy type, PhysicsInfo* info);
    void updateObjects(std::vector<GameObject*> *objects);  // ???
    void setGravity(float x, float y, float z);
    btVector3 getGravity();
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    std::vector<btCollisionShape*> collisionShapes;
    std::vector<btDefaultMotionState*> motionsStates;
    std::vector<btRigidBody*> rigidBodies;
};

#endif
