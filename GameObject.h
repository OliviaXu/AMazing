#ifndef CS248_GAMEOBJECT_H
#define CS248_GAMEOBJECT_H

#include "Framework.h"

class GameObject {
public:
    GameObject();
    Vec3 getPos();
    void draw();
    void updatePhysicalProperty(Vec3 newPos, float newMass, float newVel, float newAcc;);
private:
    struct Vec3 pos;
    GLuint shaderID;
    aiMesh* mesh;
    string texture;
    int portal;     // the portal this object is in
    float mass;
    float velocity;
    float acceleration;
    // TODO: add status field
}

#endif