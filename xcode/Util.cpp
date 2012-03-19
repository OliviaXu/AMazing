#include "Util.h"
#include "stdio.h"
#include "Framework.h"

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


void getWindowProjMat(struct MAZErectangle &viewport, struct MAZEmat &projviewMat, struct MAZEmat &viewportMat){
	MAZEmat modelviewMat;
	MAZEmat projMat;
	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMat.mat);
	glGetFloatv(GL_PROJECTION_MATRIX, projMat.mat);
	
	float farv = 1., nearv = 0.;
	viewportMat.mat[0] = viewport.width/2;
	viewportMat.mat[5] = viewport.height/2;
	viewportMat.mat[10] = (farv-nearv)/2;
	viewportMat.mat[12] = viewport.width/2;
	viewportMat.mat[13] = viewport.height/2;
	viewportMat.mat[14] = (farv + nearv)/2;

	multMat(&projMat, &modelviewMat, &projviewMat);
}

void vecAdd(struct Vec3 *vec1, struct Vec3 *vec2, struct Vec3 *vecOut){
	vecOut->x = vec1->x + vec2->x;
	vecOut->y = vec1->y + vec2->y;
	vecOut->z = vec1->z + vec2->z;
}

void inverse(struct MAZEmat *matIn, struct MAZEmat *matOut){
	float *matf = matIn->mat;
	aiMatrix4x4 mat(matf[0], matf[4], matf[8], matf[12],
					matf[1], matf[5], matf[9], matf[13],
					matf[2], matf[6], matf[10], matf[14],
					matf[3], matf[7], matf[11], matf[15]);
	mat.Inverse();

	matf = matOut->mat;
	matf[0] = mat.a1;
	matf[1] = mat.b1;
	matf[2] = mat.c1;
	matf[3] = mat.d1;

	matf[4] = mat.a2;
	matf[5] = mat.b2;
	matf[6] = mat.c2;
	matf[7] = mat.d2;

	matf[8] = mat.a3;
	matf[9] = mat.b3;
	matf[10] = mat.c3;
	matf[11] = mat.d3;

	matf[12] = mat.a4;
	matf[13] = mat.b4;
	matf[14] = mat.c4;
	matf[15] = mat.d4;
}

void normalize(struct Vec3 *v){
	float sum = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x = v->x / sum;
	v->y = v->y / sum;
	v->z = v->z / sum;
}