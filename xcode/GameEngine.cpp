//
//  GameEngine.cpp
//  CS248-Final-Project
//
//  Created by Yue Chen on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "GameEngine.h"
#include "DepthRenderTarget.h"
#include "FunctionalPortal.h"

using namespace std;

GameEngine::GameEngine(string map_file, string config_file)
{
    mapLoader = new MapLoader();
    physicsEngine = new PhysicsEngine(mapLoader);
    physicsEngine->init();
	mapLoader->load(map_file, physicsEngine);
    eventMgr = new EventMgr();
    userControl = new UserControl();
    camera = new Camera();
    plane = new Plane();
	ball = mapLoader->getBall();
	//DepthRenderTarget::init();
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
	//DepthRenderTarget::dispose();
}

void GameEngine::init(sf::Window* _window)
{
    this->window = _window;
	viewport.bottom = 0;
	viewport.left = 0;
	viewport.width = window->GetWidth();
	viewport.height = window->GetHeight();

    userControl->setWindow(window);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };
	GLfloat light_specular[] = { 0.3, 0.3, 0.3, 1. };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 1, 1, 1, 1. };
	GLfloat light1_specular[] = { 0.3, 0.3, 0.3, 1. };
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	emt = new ParticleEmitter(ParticleEmitter::PARTICLE_FIRE, "models/FireGold.png", 800);
	emt->mPosition.Set(-50/25.4, 50/25.4, 50.0/25.4);
}

void GameEngine::run()
{
    while(1){
		static float t1 = 0;
		static float t2 = t1;
		t2 = t1;
		t1 = window->GetFrameTime();
		cout << "delta time " << t1 << endl;

        userControl->handleInput();    // constant * window.GetFrameTime() 

		//what's the reason for this??? 
       
        float dAngleNS, dAngleEW;
        userControl->getAngleUpdate(dAngleNS, dAngleEW);
        
        //plane->update(dAngleNS, dAngleEW);    // no use anymore
        
        //I think updating current portal according to camera is more appropriate.
		camera->updatePos(userControl->getCamM(),userControl->getCamDirUpdate(),ball,dAngleNS, dAngleEW);//input camera movement ball direction and ball to determin camera position and direction
		/*if(mapLoader->updateCurrentPortal(&(ball->getPos()))){
			//updateObjects();
			ball->setPortal(mapLoader->getCurrentPortalIdx());
		}*/
		mapLoader->updateCurrentPortal(camera->getPos(), &(ball->getPos()));
        
        //printf("%f, %f, %f\n", GRAVITY * sin(dAngleEW/180*PI), -GRAVITY * cos(dAngleEW/180*PI) * cos(dAngleNS/180*PI), -GRAVITY * cos(dAngleEW/180*PI) * sin(dAngleNS/180*PI));
        physicsEngine->setGravity(GRAVITY * sin(dAngleEW/180*PI), -GRAVITY * cos(dAngleEW/180*PI) * cos(dAngleNS/180*PI), -GRAVITY * cos(dAngleEW/180*PI) * sin(dAngleNS/180*PI));

        physicsEngine->updateObjects();
        
        //eventMgr->updateEvents(objects, mapLoader, events);
        handleEvents();
        
		struct Vec3 lP=userControl->lightPos();
		GLfloat light_position[]={lP.x,lP.y,lP.z,0.};//directinal light;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		GLfloat light1_pos[] = {-0.5, 1, -1, 0};
		glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
		//printf("cam mov: % d light: %f %f %f\n",userControl->getCamM(),light_position[0],light_position[1],light_position[2]);
        drawScene();
        
        window->Display();
    }
}

void GameEngine::updateObjects()
{
    objects.clear();
    mapLoader->fillObjects(objects);
}

void GameEngine::onBallIntoPortal(FunctionalPortal *fp){
	struct Vec3 dst_pcoord = fp->getDestPortalPos();
	int idstportal = fp->getDestPortalIdx();
	Portal *port = (Portal *)(mapLoader->getPortal(idstportal));
	struct Vec3 dstcoord;
	struct Vec3 portalPos = port->getPos();
	vecAdd(&dst_pcoord, &portalPos, &dstcoord);
	ball->moveTo(dstcoord.x, dstcoord.y, dstcoord.z);
}

void GameEngine::handleEvents()
{
	queue<MAZEevent> *eq = physicsEngine->getEventQ();
	while(!eq->empty()){
		struct MAZEevent e = eq->front();
		eq->pop();
		switch(e.ty){
		case MAZEevent_type::BALL_INTO_PORTAL:
			onBallIntoPortal((FunctionalPortal *)e.info);
			break;
		case MAZEevent_type::BALL_INTO_HOLE:
			ball->moveTo(-250/25.4, 20010/25.4,0);
			//ball->rigidBody->setDamping(0.5, 0.5);
			ball->rigidBody->setLinearVelocity(btVector3(0, 0, 0));
			break;
		case MAZEevent_type::START_GAME:
		case MAZEevent_type::RESTART_GAME:
			ball->moveTo(-250/25.4, 200/25.4,0);
			ball->rigidBody->setLinearVelocity(btVector3(0, 0, 0));
			//ball->rigidBody->setDamping(0.5, 0.5);
			break;
		case MAZEevent_type::QUIT_GAME:
			exit(0);
			break;
		/*default:
			break;*/
		}
	}
}


#define MAX_DELAY	4.0f
#define TIMESTEP 1.f/60.f
float accumulator = 0.;
void drawParticles(sf::Window *window, ParticleEmitter *emitter) {
	float elapsedTime = window->GetFrameTime();

	// Only update/spawn every 1/60th second
	for(accumulator += min(MAX_DELAY, elapsedTime); accumulator > TIMESTEP; accumulator -= TIMESTEP) {
			emitter->updateParticles(TIMESTEP);

			// Spawn new particles
			emitter->spawnParticles(TIMESTEP);
	}
	emitter->renderParticles();
}

void GameEngine::drawScene()
{
    //const Portal *portal = mapLoader->getCurrentPortal();
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//int iRootPortal = mapLoader->getCurrentPortalIdx();
	//cout << iRootPortal << endl;
	struct MAZEmat projviewMat;
	struct MAZEmat viewportMat;

	getWindowProjMat(viewport, projviewMat, viewportMat);

	//This cast from const to non-const is pretty dirty.
	//Not sure how to call rootPortal->cullDraw without doing this...
	Portal *rootPortal = (Portal *)mapLoader->getCurrentPortal();
	int ip = mapLoader->getCurrentPortalIdx();
	if(DEBUG_OUTPUT)
        cout << "portal " << ip << endl;
	/*rootPortal->doorStatus[0] = 0;
	rootPortal->doorStatus[1] = 0;
	rootPortal->doorStatus[2] = 0;
	rootPortal->doorStatus[3] = 0;*/
	set<int *> visitedEdgeSet;
	rootPortal->cullDraw(&projviewMat, &viewportMat, viewport, 
							mapLoader->getPortals(), visitedEdgeSet);

	//drawParticles(window, emt);
}
