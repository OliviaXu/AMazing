//
//  PhysicsEngine.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "PhysicsEngine.h"
#include "SpherePhysicsInfo.h"
#include "PlanePhysicsInfo.h"

#include <iostream>
using namespace std;

PhysicsEngine::PhysicsEngine() {
}

PhysicsEngine::~PhysicsEngine() {
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
    
    for(int i = 0;i < rigidBodies.size();++i)
    {
        dynamicsWorld->removeRigidBody(rigidBodies[i]);
        delete rigidBodies[i]->getMotionState();
        delete rigidBodies[i];
    }
    for(int i = 0;i < collisionShapes.size();++i)
        delete collisionShapes[i];
}

void PhysicsEngine::init() {
    broadphase = new btDbvtBroadphase();
    
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));
}

void PhysicsEngine::addObject(PhysicsShapeTy type, PhysicsInfo* info)
{
    switch (type) {
        case PHYSP_SPHERE:
        {
            SpherePhysicsInfo* cur_info = (SpherePhysicsInfo*)info;
            btCollisionShape* sphereShape = new btSphereShape(cur_info->radius);
            btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(cur_info->trans_x, cur_info->trans_y, cur_info->trans_z, cur_info->trans_w),btVector3(cur_info->pos_x, cur_info->pos_y, cur_info->pos_z)));
            btRigidBody* sphereRigidBody;
            if(cur_info->is_static)
            {
                btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(0, sphereMotionState,sphereShape,btVector3(0,0,0));
                sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
            }
            else
            {
                btVector3 sphereInertia(0,0,0);
                sphereShape->calculateLocalInertia(cur_info->mass,sphereInertia);
                btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(cur_info->mass, sphereMotionState, sphereShape, sphereInertia);
                sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
            }
            
            dynamicsWorld->addRigidBody(sphereRigidBody);
            collisionShapes.push_back(sphereShape);
            motionsStates.push_back(sphereMotionState);
            rigidBodies.push_back(sphereRigidBody);
            break;
        }
        case PHYSP_PLANE:
        {
            PlanePhysicsInfo* cur_info = (PlanePhysicsInfo*)info;
            btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(cur_info->normal_x, cur_info->normal_y, cur_info->normal_z),0);
            btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(cur_info->trans_x, cur_info->trans_y, cur_info->trans_z, cur_info->trans_w),btVector3(cur_info->pos_x, cur_info->pos_y, cur_info->pos_z)));
            btRigidBody* planeRigidBody;
            if(cur_info->is_static)
            {
                btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(0, planeMotionState,planeShape,btVector3(0,0,0));
                planeRigidBody = new btRigidBody(planeRigidBodyCI);
            }
            else
                ;//TODO
            dynamicsWorld->addRigidBody(planeRigidBody);
            collisionShapes.push_back(planeShape);
            motionsStates.push_back(planeMotionState);
            rigidBodies.push_back(planeRigidBody);
            break;
        }
    }
}

void PhysicsEngine::updateObjects(std::vector<GameObject *> *objects) {
    dynamicsWorld->stepSimulation(1/60.f,10);
    
    for(int i = 0;i < rigidBodies.size();++i)
    {
        btTransform trans;
        rigidBodies[i]->getMotionState()->getWorldTransform(trans);
        btScalar m[15];
        trans.getOpenGLMatrix(m);

        //if(i == 7)
        //{
        cout << "Rigid Object #" << i << ": X = " << trans.getOrigin().getX() << ", Y = " << trans.getOrigin().getY() << ", Z = " << trans.getOrigin().getZ() << endl;
        /*cout << "Rigid Object #" << i << ": ";
        for(int j = 0;j < 15;++j)
            cout << m[j] << " ";
        cout << endl;*/
        //}
        
        (*objects)[i]->setTrans(m);
        btVector3 p = trans.getOrigin();
        (*objects)[i]->setPos(p.getX(), p.getY(), p.getZ());
    }
}