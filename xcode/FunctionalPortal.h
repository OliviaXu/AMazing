#ifndef CS248_FUNCTIONALPORTAL_H
#define CS248_FUNCTIONALPORTAL_H

#include "Framework.h"
#include "Portal.h"
#include "GameObject.h"
#include "DepthRenderTarget.h"

#define EYE_TO_PORTAL_DIST 2

class FunctionalPortal : public GameObject{
public:

	FunctionalPortal();
	~FunctionalPortal();
	virtual void draw(const std::vector<Portal *> *portals);
	virtual void passShaderParam(const aiMesh *mesh);
	
	void setPosDir(struct Vec3 *destPos, int dir, int idstPortal, int srcLookDir);
	struct Vec3 getDestPortalPos();
	int getDestPortalIdx();

protected:
	void bindTexture();

private:
	void createEnvironmentMap(const std::vector<Portal *> *portals);

	struct Vec3 destPos_pcoord;
	struct Vec3 eye_pos;
	int direction;
	int src_lookDirection;
	GLuint dstCubemap;
	GLuint dstDepthmap;
	int iDestPortal;
	int width;
	int height;
	DepthRenderTarget *tgr;
};

#endif