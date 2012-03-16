#include "PhysicsInfo.h"

void PhysicsInfo::parse(char *args){
	char *str;
	assert(str = strtok(args, " \t"));
	mass = atof(str)/25.4;

	assert(str = strtok(NULL, " \t"));
	is_static = atoi(str);

	assert(str = strtok(NULL, " \t"));
	trans_x = atof(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	trans_y = atof(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	trans_z = atof(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	trans_w = atof(str);

	assert(str = strtok(NULL, " \t"));
	pos_x = atof(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	pos_y = atof(str) / 25.4;

	assert(str = strtok(NULL, " \t"));
	pos_z = atof(str) / 25.4;
}

void PhysicsInfo::set(PhysicsInfo *info){
	trans_x = info->trans_x;
	trans_y = info->trans_y;
	trans_z = info->trans_z;
	trans_w = info->trans_w;

	pos_x = info->pos_x;
	pos_y = info->pos_y;
	pos_z = info->pos_z;

	is_static = info->is_static;
	mass = info->mass;

	shapeTy = info->shapeTy;
}