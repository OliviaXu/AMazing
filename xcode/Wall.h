#ifndef CS248_WALL_H
#define CS248_WALL_H

#include "Framework.h"
#include "GameObject.h"

class Wall : public GameObject{
public:
	Wall();
	~Wall();
	virtual void passShaderParam(const aiMesh *mesh);
	virtual void setTan(const aiMesh *mesh);
protected:

private:
};

#endif