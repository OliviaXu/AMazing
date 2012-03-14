#include "FunctionalPortal.h"

#include "DepthRenderTarget.h"
#include "Shader.h"
#include "Util.h"

using namespace std;

FunctionalPortal::FunctionalPortal(){
	width = 400;
	height = 400;
	iDestPortal = 3;
	destPos_pcoord.x = -2;
	destPos_pcoord.y = 3;
	destPos_pcoord.z = 1;
	lookPos.x = -4.3;
	lookPos.y = 1.6;
	lookPos.z = 1.6;

	motionNormal.x = 1;
	motionNormal.y = 0;
	motionNormal.z = 0;
	/*destLook.x = 0;
	destLook.y = 0;
	destLook.z = -10;*/

	renderTgr = new DepthRenderTarget(width, height);
	glEnable(GL_TEXTURE_CUBE_MAP);
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
}

FunctionalPortal::~FunctionalPortal(){
	delete renderTgr;
}

void FunctionalPortal::draw(const std::vector<Portal *> *portals){
	GLuint shaderID = shader->programID();
	glUseProgram(shaderID);
	struct MAZEmat inverseViewMat;
	GL_CHECK(glGetFloatv(GL_MODELVIEW_MATRIX, inverseViewMat.mat));
	inverse(&inverseViewMat, &inverseViewMat);
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE0);
	GL_CHECK(glLoadMatrixf(inverseViewMat.mat));

	struct MAZEmat viewMat;
	GL_CHECK(glGetFloatv(GL_MODELVIEW_MATRIX, viewMat.mat));
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE1);
	GL_CHECK(glLoadMatrixf(viewMat.mat));
	
	Portal *portal = (*portals)[iPortal];
	GLuint lpos = GL_CHECK(glGetUniformLocation(shaderID, "lookPosIn"));
	GLuint mnormal = GL_CHECK(glGetUniformLocation(shaderID, "motionNormalIn"));
	GLuint timeIn = GL_CHECK(glGetUniformLocation(shaderID, "timeIn"));

	struct Vec3 portalPos = portal->getPos();
	time_t t;
	time(&t);
	cout << "time " << (float)(t%100) << endl;
	GL_CHECK(glUniform3f(lpos, lookPos.x+portalPos.x, lookPos.y+portalPos.y, lookPos.z+portalPos.z));
	GL_CHECK(glUniform3f(mnormal, motionNormal.x, motionNormal.y, motionNormal.z));
	GL_CHECK(glUniform1f(timeIn, (float)(t%100)));

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
}

void FunctionalPortal::createEnvironmentMap(const std::vector<Portal *> *portals){
	Portal *destPortal = (*portals)[iDestPortal];
	Vec3 portalPos = destPortal->getPos();
	Vec3 eyePos;
	vecAdd(&destPos_pcoord, &portalPos, &eyePos);

	bool oldHide = isHidden();
	setHide(true);
	GL_CHECK(glPushAttrib(GL_VIEWPORT_BIT));
	GL_CHECK(glViewport(0, 0, width, height));

	GL_CHECK(glMatrixMode(GL_MODELVIEW));
	GL_CHECK(glPushMatrix());
	GL_CHECK(glMatrixMode(GL_PROJECTION));
	GL_CHECK(glPushMatrix());

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	renderTgr->bind();//Save viewport there

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
	//glViewport(0, 0, width, height);

	
	
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
	for(int i=0; i<6; i++){
		//cout<<"i " << i <<endl;
		struct Vec3 centerPos = vec2vecAdd(&eyePos, lookVectors[i*4], 
								lookVectors[i*4+1], lookVectors[i*4+2]);
		//struct Vec3 centerPos = vec2vecAdd(&eyePos, 0, 0, -10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyePos.x, eyePos.y, eyePos.z, 
				centerPos.x, centerPos.y, centerPos.z,
				up[i*4], up[i*4+1], up[i*4+2]);

		//GL_CHECK(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo));
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, dstCubemap, 0);
		if(GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)){
		cerr << "Invalid framebuffer configuration: ERROR CODE " 
			<< glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		exit(-1);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		struct MAZEmat projviewMat;
		struct MAZEmat viewportMat;

		getWindowProjMat(viewport, projviewMat, viewportMat);

		hash_set<int *> visitedEdgeSet;
		destPortal->cullDraw(&projviewMat, &viewportMat, viewport, 
							portals, visitedEdgeSet);
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap);
		//debug
	sf::Uint8 *pixelArray = new sf::Uint8[width*height*4];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); // to read from texture instead
	sf::Image temp(width, height, pixelArray);
	std::string filename = "envmap";
	char a[2] = {'a'+i, '\0'};
	filename.append(a);
	filename.append(".jpg");
	temp.SaveToFile(filename);
	delete pixelArray;
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

	renderTgr->unbind();

	setHide(oldHide);
}