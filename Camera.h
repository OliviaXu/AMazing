#ifndef CS248_CAMERA_H
#define CS248_CAMERA_H

#include <string.h>
#include "Framework.h"

class Camera{
public:
    Camera();
    void updatePos(Vec3 newBallPos);
    
    
    const static float near, far;
    const static float aspect_ratio;
    const static float FOV;
    const static struct Vec3 relPos;    // relative position to the ball
private:
    Vec3 pos;
};

#endif