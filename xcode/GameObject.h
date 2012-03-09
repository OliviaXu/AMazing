#ifndef CS248_GAMEOBJECT_H
#define CS248_GAMEOBJECT_H

#include "Framework.h"
#include "Shader.h"

class GameObject {
public:
    GameObject();
    ~GameObject();//TODO: Class name is generated via strdup. Free it!
    void draw();
	virtual void passShaderParam(const aiMesh *mesh, GLuint shaderID);
    void updatePhysicalProperty(Vec3 &newPos, float newMass, float newVel, float newAcc);
    void setPortal(int iPortal);
    void setShader(Shader *shader);
    void setMass(int mass);
    void setPos(struct Vec3 pos);
    void setClass(char *className);
	void setModel(const aiScene *model, std::vector<unsigned int> *indexBuff);
	void setTexture(sf::Image *dtex, sf::Image *stex);
    struct Vec3 getPos();
protected:
	MAZEmat transformation;
    struct Vec3 pos;
    Shader *shader;
    const aiScene *model;
	std::vector<unsigned int> *indexBuff;
	sf::Image *tex;

    char *className;
    sf::Image *dtex;
	sf::Image *stex;
    int iPortal;     // the portal this object is in
    float mass;
    struct Vec3 velocity;
    float acceleration;
    // TODO: add status field
};

#endif
