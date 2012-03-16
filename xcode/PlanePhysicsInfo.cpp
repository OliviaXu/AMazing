#include "PlanePhysicsInfo.h"

void PlanePhysicsInfo::parse(char *args){
	char *str;
	assert(str = strtok(args, " \t"));
	normal_x = atoi(str);
    args = str + strlen(str) + 1;

	assert(str = strtok(args, " \t"));
	normal_y = atoi(str);
    args = str + strlen(str) + 1;

	assert(str = strtok(args, " \t"));
	normal_z = atoi(str);

	PhysicsInfo::shapeTy = PHYSP_PLANE;
	args = str + strlen(str) + 1;
	PhysicsInfo::parse(args);
}

PhysicsInfo *PlanePhysicsInfo::clone(){
	PlanePhysicsInfo *info = new PlanePhysicsInfo();
	info->set(this);
	info->normal_x = normal_x;
	info->normal_y = normal_y;
	info->normal_z = normal_z;

	return info;
}