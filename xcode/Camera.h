#ifndef CS248_CAMERA_H
#define CS248_CAMERA_H

#include "Framework.h"
#include "Ball.h"
#define VOFFSET 2//vetrtical offset of camera to ball
#define HOFFSET 4//horizontal offset of camera to ball

#define PI 3.1415926


class Camera{
public:
    Camera();
    //void updatePos(MAZEorientation pOri,Ball *ball);
	//void updatePos(Keyorientation keyd,Ball *ball);//fist try... implement the ball purely according to the movement direction of the ball
    //debug version can change the camera's position
	void updatePos(CamMorientation mov,Keyorientation keyd,Ball *ball,float AngleNS, float AngleEW);
	static const float nearClip;
	static const float farClip;
	static const float aspectRatio;
    static const float FOV;
    static const struct Vec3 relPos;    // relative position to the ball
	const struct Vec3 *getPos();
private:
    struct Vec3 pos;
    struct Vec3 dir;
	Morientation camO;
	Morientation ballO;
	bool control_m;//user cotrol as false
};

#endif