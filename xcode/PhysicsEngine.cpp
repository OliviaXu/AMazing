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
#include "BoxPhysicsInfo.h"
#include "Ball.h"
#include "MapLoader.h"

#include <iostream>
using namespace std;

#define FRICTION 1.2
#define DAMPING 10.1

struct BallCollisionCallback : public btCollisionWorld::ContactResultCallback{
	BallCollisionCallback(Ball *b, vector<GameObject *> *objs, queue<MAZEevent> *evtq){
		ball = b;
		objects = objs;
		eq = evtq;
	}

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObject* colObj0,int partId0,int index0,
		const btCollisionObject* colObj1,int partId1,int index1)
	{
		btRigidBody *b0 = (btRigidBody *)colObj0;
		btRigidBody *b1 = (btRigidBody *)colObj1;
		GameObject *o1;
		if(b0->getUserPointer() == ball){
			o1 = (GameObject *)b1->getUserPointer();
		}
		else{
			assert(b1->getUserPointer() == ball);
			o1 = (GameObject *)b0->getUserPointer();
		}
		ball->respondToCollision(o1, eq);

		
		return false;
	}
	Ball *ball;
	vector<GameObject *> *objects;
	queue<MAZEevent> *eq;
};

PhysicsEngine::PhysicsEngine(MapLoader *mld) {
	this->mld = mld;
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

bool OnContactAdded(
    btManifoldPoint& cp,
    const btCollisionObject* colObj0,
    int partId0,
    int index0,
    const btCollisionObject* colObj1,
    int partId1,
    int index1){
	cout << "contact added" << endl;
	return false;
}

void PhysicsEngine::init() {
    broadphase = new btDbvtBroadphase();
    
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-GRAVITY,0));
}

void PhysicsEngine::addObject(PhysicsShapeTy type, PhysicsInfo* info, GameObject *obj)
{
	objs.push_back(obj);
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
				sphereRigidBody->setFriction(FRICTION);
                sphereRigidBody->setDamping(DAMPING, DAMPING);
            }
            else
            {
                btVector3 sphereInertia(0,0,0);
                sphereShape->calculateLocalInertia(cur_info->mass,sphereInertia);
                btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(cur_info->mass, sphereMotionState, sphereShape, sphereInertia);
                sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
				sphereRigidBody->setFriction(FRICTION);
                sphereRigidBody->setDamping(DAMPING, DAMPING);
            }
            //sphereRigidBody->forceActivationState(info->forceActivation);
			//sphereRigidBody->forceActivationState(1);
            dynamicsWorld->addRigidBody(sphereRigidBody);
            collisionShapes.push_back(sphereShape);
            motionsStates.push_back(sphereMotionState);
            rigidBodies.push_back(sphereRigidBody);
			//TODO: Remove object vector, use rigidbody pointer to obj only.
			obj->setRigidBody(sphereRigidBody);
			sphereRigidBody->setUserPointer(obj);
			Ball *is_it_ball = dynamic_cast<Ball *>(obj);
			if(is_it_ball){
				sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);
                BallCollisionCallback bcb = BallCollisionCallback(is_it_ball, &objs, &evtq);
				dynamicsWorld->contactTest(sphereRigidBody, 
                    bcb);
			}
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
				btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(cur_info->mass, planeMotionState,planeShape,btVector3(0,0,0));
                planeRigidBody = new btRigidBody(planeRigidBodyCI);
				planeRigidBody->setFriction(FRICTION);
                planeRigidBody->setDamping(DAMPING, DAMPING);
            }
            else
                ;//TODO
			//planeRigidBody->forceActivationState(info->forceActivation);
            dynamicsWorld->addRigidBody(planeRigidBody);
            collisionShapes.push_back(planeShape);
            motionsStates.push_back(planeMotionState);
            rigidBodies.push_back(planeRigidBody);
			obj->setRigidBody(planeRigidBody);
			planeRigidBody->setUserPointer(obj);
            break;
        }
		case PHYSP_BOX:
		{
			BoxPhysicsInfo *cur_info = (BoxPhysicsInfo *)info;
			btBoxShape *boxShape = new btBoxShape(btVector3(cur_info->half_x, cur_info->half_y, cur_info->half_z));
			btDefaultMotionState* boxMotionState = 
				new btDefaultMotionState(
					btTransform(btQuaternion(cur_info->trans_x, 
											cur_info->trans_y, 
											cur_info->trans_z, 
											cur_info->trans_w),
								btVector3(cur_info->pos_x, 
										cur_info->pos_y, 
										cur_info->pos_z)));
            
			btRigidBody* boxRigidBody;
            if(cur_info->is_static)
            {
				btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(cur_info->mass, boxMotionState,boxShape,btVector3(0,0,0));
                boxRigidBody = new btRigidBody(boxRigidBodyCI);
				boxRigidBody->setFriction(FRICTION);
                boxRigidBody->setDamping(DAMPING, DAMPING);
            }
            else
                ;//TODO
			//boxRigidBody->forceActivationState(info->forceActivation);
            dynamicsWorld->addRigidBody(boxRigidBody);
            collisionShapes.push_back(boxShape);
            motionsStates.push_back(boxMotionState);
            rigidBodies.push_back(boxRigidBody);
			obj->setRigidBody(boxRigidBody);
			boxRigidBody->setUserPointer(obj);
            break;
		}
    }
}

queue<MAZEevent> *PhysicsEngine::getEventQ(){
	return &evtq;
}

void PhysicsEngine::updateObjects() {
    dynamicsWorld->stepSimulation(1/60.f,10);
    for(int i = 0;i < rigidBodies.size();++i)
    {
		GameObject *obj = objs[i];
		Ball *is_it_ball = dynamic_cast<Ball *>(obj);
			if(is_it_ball){
				//rigidBodies[i]->setActivationState(DISABLE_DEACTIVATION);
                BallCollisionCallback bcb = BallCollisionCallback(is_it_ball, &objs, &evtq);
				dynamicsWorld->contactTest(rigidBodies[i], 
                    bcb);
			}

		//Modified by sylvon. Only set the ball to be always active.
		//rigidBodies[i]->setActivationState(1);

		//Modified by sylvon. Only perform matrix update when rb is active
		if(rigidBodies[i]->isActive()){
			btTransform trans;
			rigidBodies[i]->getMotionState()->getWorldTransform(trans);
			btScalar m[16];
			trans.getOpenGLMatrix(m);

        /*if(i == 10)
        {
            cout << "Rigid Object #" << i << ": X = " << trans.getOrigin().getX() << ", Y = " << trans.getOrigin().getY() << ", Z = " << trans.getOrigin().getZ() << endl;
			cout << "Rigid body " << rigidBodies[i]->getInvMass() << endl;
        }*/
			obj->setTrans(m);
			btVector3 p = trans.getOrigin();
			obj->setPos(p.getX(), p.getY(), p.getZ());
			mld->updateObjPortal(obj);
		}
    }
}

void PhysicsEngine::setGravity(float x, float y, float z)
{
    dynamicsWorld->setGravity(btVector3(x,y,z));
}