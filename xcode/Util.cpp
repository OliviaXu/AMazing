#include "Util.h"

void vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz){
	vecIn->x = vecIn->x + dx;
	vecIn->y = vecIn->y + dy;
	vecIn->z = vecIn->z + dz;
}