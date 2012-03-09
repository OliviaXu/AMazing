#include "Util.h"
#include "stdio.h"

//this is to do increment of one Vec3
void vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz){
	vecIn->x = vecIn->x + dx;
	vecIn->y = vecIn->y + dy;
	vecIn->z = vecIn->z + dz;
}

//this is to do two element addition 
struct Vec3 vec2vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz){
	 struct Vec3 summ(vecIn->x + dx,vecIn->y + dy,vecIn->z + dz);
	 //printf("summ %f %f %f \n",summ.x,summ.y,summ.z);
	 return summ;
}

//compare two vec3
bool Vequal(struct Vec3 *vecIna, struct Vec3 *vecInb){
	bool vequal=false;
	if(vecIna->x==vecInb->x && vecIna->y==vecInb->y && vecIna->z==vecInb->z){
		vequal=true;
	}
	return vequal;
}
//multiply one vec3 with a float 
void VMulti(struct Vec3 *vecIna, float num){
	vecIna->x*=num;
	vecIna->y*=num;
	vecIna->z*=num;
}