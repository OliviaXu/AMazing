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
	
	void setPosDir(struct Vec3 *destPos, int dir, int idstPortal, int srcLookDir, float look_len, int transport);
	struct Vec3 getDestPortalPos();
	int getDestPortalIdx();
	virtual float getEyeToPortalDist();
	
	
	float look_length;
	int transport;
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