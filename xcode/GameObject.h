#ifndef CS248_GAMEOBJECT_H
#define CS248_GAMEOBJECT_H

#include "Framework.h"

class GameObject {
public:
    GameObject();
    struct Vec3 getPos();
    void draw();
    void updatePhysicalProperty(Vec3 &newPos, float newMass, float newVel, float newAcc;);
private:
    struct Vec3 pos;
    Shader *shader;
    const aiScene *model;
    int iTex;
    int iPortal;     // the portal this object is in
    float mass;
    float velocity;
    float acceleration;
    // TODO: add status field
}

#endif