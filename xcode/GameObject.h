#ifndef CS248_GAMEOBJECT_H
#define CS248_GAMEOBJECT_H

#include "Framework.h"
#include "Shader.h"

class GameObject {
public:
    GameObject();
	~GameObject();//TODO: Class name is generated via strdup. Free it!
    Vec3 getPos();
    void draw();
    void updatePhysicalProperty(Vec3 newPos, float newMass, float newVel, float newAcc);
	void setPortal(int iPortal);
	void setShader(int iShader);
	void setMass(int mass);
	void setPos(Vec3 &pos);
	void setClass(char *className);

private:
    struct Vec3 pos;
    Shader *shader;
    const aiScene *model;
	char *className;
    int iTex;
    int iPortal;     // the portal this object is in
    float mass;
    float velocity;
    float acceleration;
    // TODO: add status field
}

#endif