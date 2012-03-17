#ifndef CS248_GAMEOBJECT_H
#define CS248_GAMEOBJECT_H

#include "Framework.h"
#include "Shader.h"
#include "PhysicsInfo.h"

class Portal;

class GameObject {
public:
    GameObject();
    ~GameObject();//TODO: Class name is generated via strdup. Free it!
    virtual void draw(const std::vector<Portal *> *portals);//the function I'm gonna rewrite...
	virtual void passShaderParam(const aiMesh *mesh);//the function I'm gonna rewrite
    void updatePhysicalProperty(Vec3 &newPos, float newMass, float newVel, float newAcc);
    void setPortal(int iPortal);
    void setShader(const Shader *shader);
    void setMass(int mass);
    void setPos(struct Vec3 pos);
    void setClass(char *className);
	void setModel(const aiScene *model, const std::vector<unsigned int> *indexBuff);
	void setTexture(const sf::Image *dtex, const sf::Image *stex);
	struct Vec3 getPos();
    struct Vec3 &getVelocity();
	int getPortal();
	void setHide(bool flag);
	bool isHidden();
    void setTrans(float* mat);
    void setPos(float x, float y, float z);
	PhysicsInfo *phyinfo;

protected:
	void setMaterial(const aiMesh *mesh);
	void bindTexture();
	void setMeshData(const aiMesh *mesh);

	MAZEmat transformation;
    struct Vec3 pos;
    const Shader *shader;
    const aiScene *model;
	const std::vector<unsigned int> *indexBuff;

    char *className;
    const sf::Image *dtex;
	const sf::Image *stex;
    int iPortal;     // the portal this object is in
    float mass;
    struct Vec3 velocity;
    float acceleration;
	bool hide;
    // TODO: add status field
};

#endif
