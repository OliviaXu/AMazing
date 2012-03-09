#ifndef CS248_CAMERA_H
#define CS248_CAMERA_H

#include "Framework.h"
#include "Ball.h"
#define VOFFSET 15.//vetrtical offset of camera to ball
#define HOFFSET 10.//horizontal offset of camera to ball


class Camera{
public:
    Camera();
    //void updatePos(MAZEorientation pOri,Ball *ball);
	//void updatePos(Keyorientation keyd,Ball *ball);//fist try... implement the ball purely according to the movement direction of the ball
    //debug version can change the camera's position
	void updatePos(CamMorientation mov,Keyorientation keyd,Ball *ball);
	static const float nearClip;
	static const float farClip;
	static const float aspectRatio;
    static const float FOV;
    static const struct Vec3 relPos;    // relative position to the ball
private:
    struct Vec3 pos;
    struct Vec3 dir;
	Morientation camO;
	Morientation ballO;
	
};

#endif