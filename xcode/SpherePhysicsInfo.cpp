#include "SpherePhysicsInfo.h"

void SpherePhysicsInfo::parse(char *args){
	char *str;
	assert(str = strtok(args, " \t"));
	radius = atoi(str);

	PhysicsInfo::shapeTy = PHYSP_SPHERE;
	args = str + strlen(str) + 1;
	PhysicsInfo::parse(args);
}

PhysicsInfo *SpherePhysicsInfo::clone(){
	SpherePhysicsInfo *info = new SpherePhysicsInfo();
	info->set(this);
	info->radius = radius / 25.4;

	return info;
}