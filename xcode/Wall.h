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
	//void setNormalTex(const sf::Image *normalTex);
	//void setDepthTex(const sf::Image *depthTex);
	void setNormalTex(GLuint normalTex);
	void setDepthTex(GLuint depthTex);
	void setNDTex();

protected:

private:
	//const sf::Image *normalTex;
	//const sf::Image *depthTex;
	GLuint ntexture;
	GLuint dtexture;
};
#endif