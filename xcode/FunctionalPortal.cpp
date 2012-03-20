#include "FunctionalPortal.h"

#include "DepthRenderTarget.h"
#include "Shader.h"
#include "Util.h"

using namespace std;

FunctionalPortal::FunctionalPortal(){
	width = 300;
	height = 300;
	iDestPortal = 3;

	/*destLook.x = 0;
	destLook.y = 0;
	destLook.z = -10;*/
	tgr = new DepthRenderTarget(width, height);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_2D);
	GL_CHECK(glGenTextures(1, &dstCubemap));
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap));
	GL_CHECK(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP ));
	GL_CHECK(glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP ));

	for(int i=0; i<6; i++){
		GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB8, 
			width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
	}

	/*glGenTextures(1, &dstDepthmap);
    glBindTexture(GL_TEXTURE_2D, dstDepthmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_DEPTH_COMPONENT, 
        width, 
        height, 
        0, 
        GL_DEPTH_COMPONENT, 
        GL_UNSIGNED_BYTE, 
        0);
	tgr = new DepthRenderTarget();
	tgr->init();
	tgr->bind();
	GL_CHECK(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
	GL_TEXTURE_2D, dstDepthmap, 0));
	tgr->unbind();*/
}

FunctionalPortal::~FunctionalPortal(){
}

void FunctionalPortal::draw(const std::vector<Portal *> *portals){
	GLuint shaderID = shader->programID();
	glUseProgram(shaderID);

	struct MAZEmat viewMat;
	struct MAZEmat inverseViewMat;
	GL_CHECK(glGetFloatv(GL_MODELVIEW_MATRIX, viewMat.mat));
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE1);
	GL_CHECK(glLoadMatrixf(viewMat.mat));
	
	inverse(&viewMat, &inverseViewMat);
	glActiveTexture(GL_TEXTURE0);
	GL_CHECK(glLoadMatrixf(inverseViewMat.mat));

	//Portal *portal = (*portals)[iPortal];
	GLuint lpos = GL_CHECK(glGetUniformLocation(shaderID, "lookPosIn"));
	GLuint timeIn = GL_CHECK(glGetUniformLocation(shaderID, "timeIn"));

	//struct Vec3 portalPos = portal->getPos();
	GL_CHECK(glUniform3f(lpos, eye_pos.x, eye_pos.y, eye_pos.z));
	GL_CHECK(glUniform1f(timeIn, (float)(clock()/CLOCKS_PER_SEC)));

	createEnvironmentMap(portals);

	//debug
	/*glBindTexture(GL_TEXTURE_2D, color_textureID);
	sf::Uint8 *pixelArray = new sf::Uint8[width*height*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); // to read from texture instead
	sf::Image temp(width, height, pixelArray);
	std::string filename = "envmap";
	//filename.append(ss.str());
	filename.append(".jpg");
	temp.SaveToFile(filename);*/


	GameObject::draw(portals);
}

static sf::Image default_tex(1,1,sf::Color(255, 255, 255));
void FunctionalPortal::bindTexture(){
	GLuint shaderID = shader->programID();

	//Bind diffuse map
	GLint diffuseMap = GL_CHECK(glGetUniformLocation(shaderID, "diffuseMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	if(dtex){
		dtex->Bind();
	}
	else{
		default_tex.Bind();
	}
	GL_CHECK(glUniform1i(diffuseMap, 0)); // The diffuse map will be GL_TEXTURE0

	//Bind environment map
	GLint dstEnvMap = GL_CHECK(glGetUniformLocation(shaderID, "environmentMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE2));
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap));
	GL_CHECK(glUniform1i(dstEnvMap, 2));

	//Bind specular map
	GLint specularMap = GL_CHECK(glGetUniformLocation(shaderID, "specularMap"));
	GL_CHECK(glActiveTexture(GL_TEXTURE1));
	if(stex)
		stex->Bind();
	else
		default_tex.Bind();
	GL_CHECK(glUniform1i(specularMap, 1));	
}

void FunctionalPortal::passShaderParam(const aiMesh *mesh){
	setMaterial(mesh);
	bindTexture();
	setMeshData(mesh);
	

	GLuint shaderID = shader->programID();
	GLint tangent = GL_CHECK(glGetAttribLocation(shaderID, "tangentIn"));
    GL_CHECK(glEnableVertexAttribArray(tangent)); 
	GL_CHECK(glVertexAttribPointer(tangent, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTangents));

	GLint bitangent = GL_CHECK(glGetAttribLocation(shaderID, "bitangentIn"));
    GL_CHECK(glEnableVertexAttribArray(bitangent)); 
	GL_CHECK(glVertexAttribPointer(bitangent, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mBitangents));
}

void FunctionalPortal::createEnvironmentMap(const std::vector<Portal *> *portals){
	Portal *destPortal = (*portals)[iDestPortal];
	Vec3 portalPos = destPortal->getPos();
	Vec3 eyePos;
	vecAdd(&destPos_pcoord, &portalPos, &eyePos);

	bool oldHide = isHidden();
	setHide(true);

	GL_CHECK(glMatrixMode(GL_MODELVIEW));
	GL_CHECK(glPushMatrix());
	GL_CHECK(glMatrixMode(GL_PROJECTION));
	GL_CHECK(glPushMatrix());
	GL_CHECK(glPushAttrib(GL_VIEWPORT_BIT));

	//GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	tgr->bind();
	//GL_CHECK(glBindTexture(GL_TEXTURE_2D, dstDepthmap));
	//GL_CHECK(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
	//GL_TEXTURE_2D, dstDepthmap, 0));
	/*GLuint id = renderTgr->genColorTexutre();
	glFramebufferTexture2DEXT(
        GL_FRAMEBUFFER_EXT,
        GL_COLOR_ATTACHMENT0_EXT,
        GL_TEXTURE_2D,
        id,
        0);
		struct MAZErectangle viewport;
	viewport.left = 0;
	viewport.bottom = 0;
	viewport.width = width;
	viewport.height = height;
	struct Vec3 centerPos = vec2vecAdd(&eyePos, 0, 0, -10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyePos.x, eyePos.y, eyePos.z, 
				centerPos.x, centerPos.y, centerPos.z,
				0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		struct MAZEmat projviewMat;
		struct MAZEmat viewportMat;

		getWindowProjMat(viewport, projviewMat, viewportMat);

		hash_set<int *> visitedEdgeSet;
		destPortal->cullDraw(&projviewMat, &viewportMat, viewport, 
							portals, visitedEdgeSet);
		
		glBindTexture(GL_TEXTURE_2D, id);
		//debug
	sf::Uint8 *pixelArray = new sf::Uint8[width*height*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); // to read from texture instead
	sf::Image temp(width, height, pixelArray);
	std::string filename = "envmap";
	//char a[2] = {'a'+i, '\0'};
	//filename.append(a);
	filename.append(".jpg");
	temp.SaveToFile(filename);
	return;*/

	glMatrixMode(GL_PROJECTION);
	GLfloat aspectRatio = 1.;
    GLfloat nearClip = 0.1f;
    GLfloat farClip = 400.0f;
    GLfloat fieldOfView = 90.0f; // Degrees

    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
	glViewport(0, 0, width, height);

	float lookVectors[24] = {
		1, 0, 0, 1,
		-1, 0, 0, 1,
		0, 1, 0, 1,
		0, -1, 0, 1,
		0, 0, 1, 1,
		0, 0, -1, 1
	};
	
	float up[24] = {
		0, -1, 0, 1,
		0, -1, 0, 1,
		0, 0, 1, 1,
		0, 0, -1, 1,
		0, -1, 0, 1,
		0, -1, 0, 1
	};

	struct MAZErectangle viewport;
	viewport.left = 0;
	viewport.bottom = 0;
	viewport.width = width;
	viewport.height = height;
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap));
	struct Vec3 centerPos = vec2vecAdd(&eyePos, 
										lookVectors[direction*4], 
										lookVectors[direction*4+1], 
										lookVectors[direction*4+2]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GL_CHECK(gluLookAt(eyePos.x, eyePos.y, eyePos.z, 
				centerPos.x, centerPos.y, centerPos.z,
				up[direction*4], up[direction*4+1], up[direction*4+2]));
	GL_CHECK(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X+src_lookDirection, dstCubemap, 0));
	/*if(GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)){
		cerr << "Invalid framebuffer configuration: ERROR CODE " 
			<< glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		exit(-1);
	}*/
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	struct MAZEmat projviewMat;
	struct MAZEmat viewportMat;

	getWindowProjMat(viewport, projviewMat, viewportMat);

	set<int *> visitedEdgeSet;
	destPortal->cullDraw(&projviewMat, &viewportMat, viewport, 
						portals, visitedEdgeSet);
		
	//GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap));
	/*	//debug
	sf::Uint8 *pixelArray = new sf::Uint8[width*height*4];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X+src_lookDirection, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); // to read from texture instead
	sf::Image temp(width, height, pixelArray);
	std::string filename = "envmap";
	char a[2] = {'a'+src_lookDirection, '\0'};
	filename.append(a);
	filename.append(".jpg");
	temp.SaveToFile(filename);
	delete pixelArray;*/

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	tgr->unbind();

	setHide(oldHide);
}

void FunctionalPortal::setPosDir(struct Vec3 *destPos, int dir, int idstPortal, int srcLookDir, float look_len, int transport){
	destPos_pcoord.x = destPos->x;
	destPos_pcoord.y = destPos->y;
	destPos_pcoord.z = destPos->z;
	direction = dir;

	/*
	 * Dir
	 * 0->left, 1->right, 2->top, 3->bottom, 4->front, 5->back
	 * This shall remind you of the cube map order
	 */
	float eye_offset[18] = {
		-1, 0, 0,
		1, 0, 0,
		0, -1, 0,
		0, 1, 0,
		0, 0, -1,
		0, 0, 1
	};

	eye_pos = pos;
	src_lookDirection = srcLookDir;
	iDestPortal = idstPortal;
	this->transport = transport;
	this->look_length = look_len;
	vecAdd(&eye_pos, 
			getEyeToPortalDist() * eye_offset[srcLookDir*3], 
			getEyeToPortalDist() * eye_offset[srcLookDir*3+1], 
			getEyeToPortalDist() * eye_offset[srcLookDir*3+2]);
	/*vecAdd(&eye_pos, 
			EYE_TO_PORTAL_DIST * eye_offset[dir*3], 
			EYE_TO_PORTAL_DIST * eye_offset[dir*3+1], 
			EYE_TO_PORTAL_DIST * eye_offset[dir*3+2]);*/
}

float FunctionalPortal::getEyeToPortalDist(){
	//return 2;
	return look_length;
}

struct Vec3 FunctionalPortal::getDestPortalPos(){
	return destPos_pcoord;
}

int FunctionalPortal::getDestPortalIdx(){
	return iDestPortal;
}