#ifndef CS248_CAMERA_H
#define CS248_CAMERA_H

#include "Framework.h"

class Camera{
public:
    Camera();
    void updatePos(struct Vec3 &newBallPos);
    static const float nearClip;
	static const float farClip;
	static const float aspectRatio;
    static const float FOV;
    static const struct Vec3 relPos;    // relative position to the ball
private:
    struct Vec3 pos;
    struct Vec3 dir;
};

#endif