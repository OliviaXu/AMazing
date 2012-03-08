#ifndef CS248_CAMERA_H
#define CS248_CAMERA_H

#include <string.h>
#include "Framework.h"

class Camera{
public:
    Camera();
    void updatePos(Vec3 &newBallPos);
    
    const static float near, far;
    const static float aspectRatio;
    const static float FOV;
    const static struct Vec3 relPos;    // relative position to the ball
private:
    struct Vec3 pos;
    struct Vec3 dir;
};

#endif