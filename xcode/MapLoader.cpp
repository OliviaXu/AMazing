#include "MapLoader.h"
using namespace std;

typedef struct{
	Vec3 *pos;
	int currentPortal;
} DetectPortalBundle;

typedef struct{
	int iPortal;
	int iShader;
	int iMesh;
	int x, y, z;
	int mass;
	char *className;
} ObjProp;


MapLoader::MapLoader(){
	visitBuff = NULL;
    currentPortal = 0;//The map file must supply at least one portal.
}

MapLoader::~MapLoader(){
	if(visitBuff)
		delete visitBuff;

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
	int numMeshes = 1;
	for(int k=0; k<numMeshes; k++){
		if(model->mMeshes[k]->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			continue;

		int numFaces = model->mMeshes[k]->mNumFaces;
		vector<unsigned int> *buff = new vector<unsigned int>();
		aiFace *faces = model->mMeshes[k]->mFaces;
		for(int j=0; j<numFaces; j++){
			for(int v=0; v<3; v++){
				buff->push_back(faces[j].mIndices[v]);
				//buff->push_back(0);
			}
		}
		indexBuff.push_back(buff);
	}
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

void MapLoader::readPortal(){
	Portal *p = new Portal();

	int *pNeighbors = p->getNeighbors();
	for(int i=0; i<4; i++){
		char *neighbor = strtok(NULL, " \t");
		assert(neighbor);

		if(strcmp(neighbor, "NULL") == 0)
			pNeighbors[i] = 0;
		else
			pNeighbors[i] = atoi(neighbor);
	}

	portals.push_back(p);
}

ObjProp *parseObjectLine(){
	ObjProp *prop = new ObjProp();
	int numProp = 8;
	char *str;

	assert(str = strtok(NULL, " \t"));
	prop->iMesh = atoi(str);

	assert(str = strtok(NULL, " \t"));
	prop->iPortal = atoi(str);
	
	assert(str = strtok(NULL, " \t"));
	prop->x = atoi(str);

	assert(str = strtok(NULL, " \t"));
	prop->y = atoi(str);

	assert(str = strtok(NULL, " \t"));
	prop->z = atoi(str);

	assert(str = strtok(NULL, " \t"));
	prop->className = strdup(str);

	assert(str = strtok(NULL, " \t"));
	prop->iShader = atoi(str);

	assert(str = strtok(NULL, " \t"));
	prop->mass = atoi(str);

	return prop;
}

void MapLoader::readObject(bool portalObject){
	GameObject *obj = new GameObject();
	
	ObjProp *prop = parseObjectLine();
	assert(portals.size() > prop->iPortal);
	obj->setPortal(prop->iPortal);

	obj->setMass(prop->mass);

    Vec3 pos = Vec3(prop->x, prop->y, prop->z);
	obj->setPos(pos);

	assert(shaders.size() > prop->iShader);
	obj->setShader(shaders[prop->iShader]);

	obj->setClass(prop->className);

	assert(models.size() > prop->iMesh);
	obj->setModel(models[prop->iMesh], indexBuff[prop->iMesh]);//Here we make the
											//assumption that each model contains only
											//one mesh

	objs.push_back(obj);
	if(portalObject)
		portals[prop->iPortal]->setPortalObject(obj);
	else
		portals[prop->iPortal]->addObject(obj);

	delete prop;
}

void MapLoader::load(string map_file){
	ifstream in(map_file.c_str(), ifstream::in);
	if(!in){
		cout << "Failed to load map file" << endl;
		while(1);
		exit(1);
	}
	while(!in.eof()){
		string line;
		getline(in, line);	
		
		if(line.empty() || line[0] == '#')
			continue;

		char *l = strdup(line.c_str());
		char *str;
		assert(str = strtok(l, " \t"));
		if(strcmp(str, "m") == 0)
			readModel();
		else if(strcmp(str, "s") == 0)
			loadShader();
		else if(strcmp(str, "p") == 0)
			readPortal();
		else if(strcmp(str, "pobj") == 0)
			readObject(true);
		else if(strcmp(str, "obj") == 0)
			readObject(false);

		free(l);
	}

	assertMapValidity();
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
		if(visitBuff[iPortal])
			continue;
		
		Portal *p = portals[iPortal];
		if(!fun(p, iPortal, auxData))
			return;

		int *neighbors = p->getNeighbors();
		for(int i=0; i<4; i++){
			if(neighbors[i])
				q.push(neighbors[i]);
		}
	}
}

bool detectCurrentPortal(Portal *portal, int iPortal, void *auxData){
	DetectPortalBundle *bundle = (DetectPortalBundle *)auxData;
	if(portal->in(bundle->pos)){
		bundle->currentPortal = iPortal;
		return false;
	}

	return true;
}

bool MapLoader::updateCurrentPortal(Vec3 &pos){
	//Just to make the rendering work for now
	return false;

	DetectPortalBundle bundle = {&pos, -1};
	iteratePortals(currentPortal, detectCurrentPortal, &bundle);
	if(bundle.currentPortal == currentPortal)
		return false;

	currentPortal = bundle.currentPortal;
	return true;
}

void MapLoader::fillObjects(vector<GameObject*> &objects){
	
}

Portal *MapLoader::getCurrentPortal(){
	return portals[currentPortal];
}