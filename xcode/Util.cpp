#include "Util.h"
#include "stdio.h"

//this is to do increment of one Vec3
void vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz){
	vecIn->x = vecIn->x + dx;
	vecIn->y = vecIn->y + dy;
	vecIn->z = vecIn->z + dz;
}

void setVec3(struct Vec3 *vecIn, struct Vec3 *vecOut){
	vecOut->x = vecIn->x;
	vecOut->y = vecIn->y;
	vecOut->z = vecIn->z;
}

void matMultVec3_normalize(struct MAZEmat *matIn, struct Vec3 *vecIn, struct Vec3 *vecOut){
	float *mat = matIn->mat;
	float x = mat[0]*vecIn->x + mat[4]*vecIn->y + mat[8]*vecIn->z + mat[12];
	float y = mat[1]*vecIn->x + mat[5]*vecIn->y + mat[9]*vecIn->z + mat[13];
	float z = mat[2]*vecIn->x + mat[6]*vecIn->y + mat[10]*vecIn->z + mat[14];

	float w = mat[3]*vecIn->x + mat[7]*vecIn->y + mat[11]*vecIn->z + mat[15];
	
	vecOut->x = x/w;
	vecOut->y = y/w;
	vecOut->z = z/w;
}

void multMat(struct MAZEmat *mat1, struct MAZEmat *mat2, struct MAZEmat *matout){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			matout->mat[j+i*4] = mat1->mat[j]*mat2->mat[i*4] + 
								mat1->mat[j+4]*mat2->mat[1+i*4] +
								mat1->mat[j+8]*mat2->mat[2+i*4] + 
								mat1->mat[j+12]*mat2->mat[3+i*4];
		}
	}
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