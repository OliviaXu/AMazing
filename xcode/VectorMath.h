#ifndef VECTORMATH_H
#define VECTORMATH_H

#include "Framework.h"

inline float getRand(const float minF, const float maxF) {
	return (float) rand() / (float) RAND_MAX * (maxF-minF) + minF;
}

inline float dot(const aiVector3D v1, const aiVector3D v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline aiVector3D cross(const aiVector3D v1, const aiVector3D v2) {
	aiVector3D result;
	result.x = v1.y*v2.z - v2.y*v1.z;
	result.y = v1.z*v2.x - v2.z*v1.x;
	result.z = v1.x*v2.y - v2.x*v1.y;
	return result;
}

inline void matrixMult(const float *a, const float *b, float *c) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			float sum = 0;
			for(int k = 0; k < 4; ++k) {
				sum += a[k*4 + j] * b[i*4 + k];
			}
			c[i*4 + j] = sum;
		}
	}
}

#endif
