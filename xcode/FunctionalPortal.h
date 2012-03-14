#ifndef CS248_FUNCTIONALPORTAL_H
#define CS248_FUNCTIONALPORTAL_H

#include "Framework.h"
#include "Portal.h"
#include "GameObject.h"
#include "DepthRenderTarget.h"

class FunctionalPortal : public GameObject{
public:
	FunctionalPortal();
	~FunctionalPortal();
	virtual void draw(const std::vector<Portal *> *portals);
	virtual void passShaderParam(const aiMesh *mesh);
protected:
	void bindTexture();

private:
	void createEnvironmentMap(const std::vector<Portal *> *portals);

	DepthRenderTarget *renderTgr;
	GLuint dstCubemap;
	int iDestPortal;
	int width;
	int height;
	struct Vec3 destPos_pcoord;
	struct Vec3 motionNormal;
	struct Vec3 lookPos;//In world coordinate. Used to obtain sample ray for cubemap
};

#endif