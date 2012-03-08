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
}

MapLoader::~MapLoader(){
	if(visitBuff)
		delete visitBuff;

	for(int i=0; i<importers.size(); i++){
		importers[i]->FreeScene();
		delete importers[i];
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
}

void MapLoader::loadShader(){
	char *shaderPath = strtok(NULL, " \t");
	Shader *shader = new Shader(shaderPath);
	if(!shader->loaded()){
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << "Path: " << shaderPath << endl;
		std::cerr << shader->errors() << std::endl;
		while(1);
		exit(-1);
	}
}

void MapLoader::readPortal(){
	Portal *p = new Portal();

	int *pNeighbors = p->getNeighbors();
	for(int i=0; i<4; i++){
		char *neighbor = strtok(NULL, " \t");
		
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

	str = strtok(NULL, " \t");
	prop->iMesh = atoi(str);

	str = strtok(NULL, " \t");
	prop->iPortal = atoi(str);
	
	str = strtok(NULL, " \t");
	prop->x = atoi(str);

	str = strtok(NULL, " \t");
	prop->y = atoi(str);

	str = strtok(NULL, " \t");
	prop->z = atoi(str);

	str = strtok(NULL, " \t");
	prop->className = strdup(str);

	str = strtok(NULL, " \t");
	prop->iShader = atoi(str);

	str = strtok(NULL, " \t");
	prop->mass = atoi(str);
}

void MapLoader::readPortalObject(){
	GameObject *obj = new GameObject();
}

void MapLoader::readObject(bool portalObject){
	GameObject *obj = new GameObject();
	
	ObjProp *prop = parseObjectLine();
	obj->setPortal(prop->iPortal);
	obj->setMass(prop->mass);
    Vec3 pos = Vec3(prop->x, prop->y, prop->z);
	obj->setPos(pos);
	obj->setShader(prop->iShader);
	obj->setClass(prop->className);
	objs.push_back(obj);
	if(portalObject)
		portals[prop->iPortal]->setPortalObject(obj);
	else
		portals[prop->iPortal]->addObject(obj);
}

void MapLoader::load(string map_file){
	ifstream in(map_file.c_str(), ifstream::in);
	while(!in.eof()){
		string line;
		getline(in, line);
		
		if(line.empty() || line[0] == '#')
			continue;

		char *l = strdup(line.c_str());
		char *str;
		str = strtok(l, " \t");//Not sure how strtok operates. Need further testing.
		if(str == "m")
			readModel();
		else if(str == "s")
			loadShader();
		else if(str == "p")
			readPortal();
		else if(str == "pobj")
			readObject(true);
		else if(str == "obj")
			readObject(false);

		free(l);
	}
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
	DetectPortalBundle bundle = {&pos, -1};
	iteratePortals(currentPortal, detectCurrentPortal, &bundle);
	if(bundle.currentPortal == currentPortal)
		return false;
	return true;
}

void MapLoader::fillObjects(vector<GameObject*> &objects){
	
}

Portal *MapLoader::getCurrentPortal(){
	return portals[currentPortal];
}