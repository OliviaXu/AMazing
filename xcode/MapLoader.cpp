#include "MapLoader.h"
#include "GameObjectFactory.h"
#include "SpherePhysicsInfo.h"
#include "PlanePhysicsInfo.h"
#include "BoxPhysicsInfo.h"

using namespace std;

typedef struct{
	const struct Vec3 *pos;
	int currentPortal;
} DetectPortalBundle;

typedef struct{
	int iPortal;
	int iShader;
	int iMesh;
	int x, y, z;
	int mass;
	int iDTex;
	int iSTex;
	char *className;
} ObjProp;

MapLoader::MapLoader(){
	visitBuff = NULL;
	ball = NULL;
    currentPortal = 0;//The map file must supply at least one portal.
}

MapLoader::~MapLoader(){
	if(visitBuff)
		delete visitBuff;

	delete ball;

	for(int i=0; i<importers.size(); i++){
		importers[i]->FreeScene();
		delete importers[i];
	}

	for(int i=0; i<indexBuff.size(); i++){
		delete indexBuff[i];
	}

	for(int i=0; i<shaders.size(); i++){
		delete shaders[i];
	}

	for(int i=0; i<portals.size(); i++){
		delete portals[i];
	}

	for(int i=0; i<objs.size(); i++){
		delete objs[i];
	}
	/*
	for(int i=0; i<textures.size(); i++){
		delete textures[i];
	}
	*/
	/*
	for(int i=0; i<phyinfos.size(); i++){
		delete phyinfos[i];
	}*/
}

void MapLoader::readModel(){
	char *modelPath = strtok(NULL, " \t");
	assert(modelPath);
	Assimp::Importer *importer = new Assimp::Importer();
	const aiScene *model = importer->ReadFile(modelPath, aiProcess_CalcTangentSpace |
							aiProcess_Triangulate |
							aiProcess_JoinIdenticalVertices |
							aiProcessPreset_TargetRealtime_Quality);
	if (!model || model->mNumMeshes <= 0) {
		std::cerr << importer->GetErrorString() << std::endl;
		delete importer;
	}
	models.push_back(model);
	importers.push_back(importer);

	//Construct index buffer. We assume that each aiScene contains only one mesh
	aiNode *n = model->mRootNode->mChildren[0];
	int k = n->mMeshes[n->mNumMeshes-1];
	/*int numMeshes = 1;
	for(int k=0; k<numMeshes; k++){
		if(model->mMeshes[k]->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			continue;*/
	//cout<<"pt " <<model->mMeshes[k]->mPrimitiveTypes<<endl;
	aiMesh *mes = model->mMeshes[k];
		int numFaces = model->mMeshes[k]->mNumFaces;
		vector<unsigned int> *buff = new vector<unsigned int>();
		aiFace *faces = model->mMeshes[k]->mFaces;
		for(int j=0; j<numFaces; j++){
			assert(faces[j].mNumIndices ==3 );
			for(int v=0; v<3; v++){
				buff->push_back(faces[j].mIndices[v]);
				//buff->push_back(0);
			}
		}
		indexBuff.push_back(buff);
	//}

/*	aiVector3D *mesh = model->mMeshes[0]->mVertices;
	int n = model->mMeshes[0]->mNumVertices;
	int maxx = 0x80000000;
	int minx = 0x7fffffff;
	for(int i=0; i<n; i++){
		//if(mesh[i].x > maxx)
		cout << mesh[i].x << " " ;
		if(mesh[i].x< minx)
			minx = mesh[i].x;
	}

	cout << minx << endl;*/
}

void MapLoader::loadShader(){
	char *shaderPath = strtok(NULL, " \t");
	assert(shaderPath);
	Shader *shader = new Shader(shaderPath);
	if(!shader->loaded()){
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << "Path: " << shaderPath << endl;
		std::cerr << shader->errors() << std::endl;
		while(1);
		exit(-1);
	}
	shaders.push_back(shader);
}

void MapLoader::computePortalPos(Portal *p){
	if(portals.empty()){
		p->setPos(0, 0, 0);
		return;
	}

	int idx = portals.size();

	//Search for precedent portal
	int *neighbors = p->getNeighbors();
	int i;
	for(i=0; i<4; i++){
		if(neighbors[i] >= 0 && neighbors[i] < idx){
			break;
		}
	}
	assert(i < 4);

	Portal *prevPortal = portals[neighbors[i]];
	struct Vec2 size = p->getSize();
	struct Vec3 pos;
	switch(i){
	case 0:
		pos = prevPortal->getSW();
		vecAdd(&pos, 0, 0, -size.y);
		break;
	case 1:
		pos = prevPortal->getNW();
		break;
	case 2:
		pos = prevPortal->getSE();
		break;
	case 3:
		pos = prevPortal->getSW();
		vecAdd(&pos, size.x, 0, 0);
		break;
	default:
		assert(0);
	}

	p->setPos(pos.x, pos.y, pos.z);

}

void MapLoader::readPortal(){
	Portal *p = new Portal();

	int *pNeighbors = p->getNeighbors();
	for(int i=0; i<4; i++){
		char *neighbor = strtok(NULL, " \t");
		assert(neighbor);

		if(strcmp(neighbor, "NULL") == 0)
			pNeighbors[i] = -1;
		else
			pNeighbors[i] = atoi(neighbor);
	}

	int w, h;
	char *str = strtok(NULL, " \t");
	w = atoi(str);
	str = strtok(NULL, " \t");
	h = atoi(str);
	p->setSize(w/25.4, h/25.4);

	computePortalPos(p);
	portals.push_back(p);
}

void MapLoader::readObject(bool portalObject, PhysicsEngine *engine){
	char *str;
	assert(str = strtok(NULL, " \t"));
	
	//Extract the remaining arguments and pass it to factory. There,
	//game object makers will parse the arguments and produce 
	//object according to its name.
	char *args = str+strlen(str)+1;
	GameObject *obj = factory.produce(str, args, this);

	if(!ball)
		ball = dynamic_cast<Ball *>(obj);

	objs.push_back(obj);
	if(portalObject)
		portals[obj->getPortal()]->setPortalObject(obj);
	else
		portals[obj->getPortal()]->addObject(obj);
    
    engine->addObject(obj->phyinfo->shapeTy, obj->phyinfo, obj);
}

void MapLoader::readTexture(){
	char *texPath;
	assert(texPath = strtok(NULL, " \t"));
	sf::Image *tex = new sf::Image();
	//tex->Bind();
	GLuint texturen;
	glGenTextures( 1, &texturen );
	glBindTexture( GL_TEXTURE_2D,texturen );
	//GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	assert(tex->LoadFromFile(texPath));
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,tex->GetWidth(),tex->GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,tex->GetPixelsPtr());
	//textures.push_back(tex);
	textures.push_back(texturen);
}

void MapLoader::load(string map_file, PhysicsEngine *engine){
	ifstream in(map_file.c_str(), ifstream::in);
	if(!in){
		//cout << "Failed to load map file" << endl;
		while(1);
		exit(1);
	}
	while(!in.eof()){
		string line;
		getline(in, line);	
		
		if(line.empty() || line[0] == '#')
			continue;

		char *tmp = (char *)line.c_str();
		char *l = strdup(line.c_str());
		char *str;
		assert(str = strtok(l, " \t"));
		if(strcmp(str, "m") == 0)
			readModel();
		else if(strcmp(str, "s") == 0)
			loadShader();
		else if(strcmp(str, "p") == 0)
			readPortal();
		else if(strcmp(str, "t") == 0)
			readTexture();
		else if(strcmp(str, "phy") == 0)
			readPhyInfo();
		else if(strcmp(str, "pobj") == 0)
			readObject(true, engine);
		else if(strcmp(str, "obj") == 0)
			readObject(false, engine);

		free(l);
	}

	assertMapValidity();
}

void MapLoader::readPhyInfo(){
	PhysicsInfo *info;

	char *str;
	assert(str = strtok(NULL, " \t"));
	if(strcmp(str, "plane") == 0)
		info = new PlanePhysicsInfo();
	else if(strcmp(str, "sphere") == 0)
		info = new SpherePhysicsInfo();
    else if(strcmp(str, "box") == 0)
        info = new BoxPhysicsInfo();

	str = str + strlen(str) + 1;
	info->parse(str);
	phyinfos.push_back(info);
}

void MapLoader::assertMapValidity(){
	assert(!portals.empty());
	assert(!models.empty());
	assert(!objs.empty());
	assert(!shaders.empty());
}

void MapLoader::iteratePortals(int rootIdx, PortalIterateFun fun, void *auxData){
	if(!visitBuff)
		visitBuff = new bool[portals.size()];
	memset(visitBuff, 0, sizeof(bool) * portals.size());
	queue<int> q;
	q.push(rootIdx);
	while(!q.empty()){
		int iPortal = q.front();
		q.pop();
		visitBuff[iPortal] = true;

		Portal *p = portals[iPortal];
		MAZEportal_cull cull = fun(p, iPortal, auxData);
		if(cull == CULL_REST)
			return;
		else if(cull == CULL_NEIGHBORS)
			continue;

		int *neighbors = p->getNeighbors();
		for(int i=0; i<4; i++){
			if(neighbors[i] >= 0 && !visitBuff[neighbors[i]])
				q.push(neighbors[i]);
		}
	}
}

MAZEportal_cull detectCurrentPortal(Portal *portal, int iPortal, void *auxData){
	DetectPortalBundle *bundle = (DetectPortalBundle *)auxData;
	if(portal->in(bundle->pos)){
		bundle->currentPortal = iPortal;
		return CULL_REST;
	}

	return CULL_NONE;
}

bool MapLoader::updateCurrentPortal(const struct Vec3 *campos, const struct Vec3 *ballPos){
	//Just to make the rendering work for now
	//return false;

	DetectPortalBundle bundle = {campos, -1};
	iteratePortals(currentPortal, detectCurrentPortal, &bundle);

	if(bundle.currentPortal < 0)
		bundle.currentPortal = ball->getPortal();

	if(bundle.currentPortal == currentPortal)
		return false;
	currentPortal = bundle.currentPortal;
	currentOrient = portals[currentPortal]->getOrientation();
	return true;
}

void MapLoader::fillObjects(vector<GameObject*> &objects){
	
}

const Portal *MapLoader::getCurrentPortal(){
	return portals[currentPortal];
}

int MapLoader::getCurrentPortalIdx(){
	return currentPortal;
}

MAZEorientation MapLoader::getCurrentOrientation(){
	return currentOrient;
}

const vector<Portal *> *MapLoader::getPortals(){
	return &portals;
}

const aiScene *MapLoader::getModel(int iModel){
	return models[iModel];
}

const std::vector<unsigned int> *MapLoader::getIndexBuff(int iBuff){
return indexBuff[iBuff];
}

//const sf::Image *MapLoader::getTexture(int iTex){
const GLuint MapLoader::getTexture(int iTex){
	return textures[iTex];
}

const Portal *MapLoader::getPortal(int iPortal){
	return portals[iPortal];
}

const Shader *MapLoader::getShader(int iShader){
	return shaders[iShader];
}

std::vector<GameObject *>* MapLoader::getObject()
{
    return &objs;
}

Ball *MapLoader::getBall(){
	return ball;
}

void MapLoader::updateObjPortal(GameObject *obj){
	struct Vec3 pos = *(obj->getPos());
	DetectPortalBundle bundle = {&pos, 0};
	int objCurrentPortal = obj->getPortal();
	iteratePortals(objCurrentPortal, detectCurrentPortal, &bundle);
	if(bundle.currentPortal == objCurrentPortal)
		return;

	//Portal changed
	portals[objCurrentPortal]->removeObject(obj);
	portals[bundle.currentPortal]->addObject(obj);
	obj->setPortal(bundle.currentPortal);
}

void MapLoader::setAlpha(float alpha)
{
    //for(int i = 0;i < shaders.size();++i)
    {
        int i = 0;
        GLuint shaderId = shaders[i]->programID();
        GLuint	alphaId = glGetAttribLocation(shaderId, "alphaIn");
        glVertexAttrib1f(alphaId, alpha);
    }
}