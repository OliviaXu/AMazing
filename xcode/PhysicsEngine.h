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
#include "Framework.h"
class MapLoader;

#define GRAVITY 30

class PhysicsEngine {
public:
    PhysicsEngine(MapLoader *mld);
    ~PhysicsEngine();
    void init();
    void addObject(PhysicsShapeTy type, PhysicsInfo* info, GameObject *obj);
    void updateObjects();  // ???
    void setGravity(float x, float y, float z);
    btVector3 getGravity();
	std::queue<MAZEevent> *getEventQ();
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    std::vector<btCollisionShape*> collisionShapes;
    std::vector<btDefaultMotionState*> motionsStates;
    std::vector<btRigidBody*> rigidBodies;
	std::vector<GameObject *> objs;
	std::queue<MAZEevent> evtq;

	MapLoader *mld;
};

#endif
