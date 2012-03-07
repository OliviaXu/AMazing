#include "MapLoader.h"

MapLoader::MapLoader(){
	visitBuff = NULL;
}

MapLoader::~MapLoader(){
	if(visitBuff)
		delete visitBuff;
}

void MapLoader::load(std::string map_file){

}

void MapLoader::iteratePortals(int rootIdx, PortalIterateFun fun, void *auxData){
	memset(visitBuff, 0, sizeof(bool) * portals.size());
	std::queue<int> q;
	q.push(rootIdx);
	while(!q.empty()){
		int iPortal = q.front();
		q.pop();
		if(visitBuff[iPortal])
			continue;
		
		Portal *p = portals[iPortal];
		if(!fun(p, auxData))
			return;

		int *neighbors = p->getNeighbors();
		for(int i=0; i<4; i++){
			if(neighbors[i])
				q.push[neighbors[i]];
		}
	}
}

bool MapLoader::updateCurrentPortal(Vec3 pos){
	memset(visitBuff, 0, sizeof(bool) * portals.size());

	int iPortal = currentPortal;
	Portal p = portals[currentPortal];
	
}

void MapLoader::fillObjects(std::vector<GameObject> &objects){
	
}

Portal MapLoader::getCurrentPortal(){
	return portals[currentPortal];
}