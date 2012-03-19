#ifndef CS248_UTIL_H
#define CS248_UTIL_H

enum Morientation{NORTH,EAST,SOUTH,WEST};
enum Keyorientation{UP,RIGHT,DOWN,LEFT};
enum CamMorientation{NO,U,D,L,R,F,B};


struct MAZErectangle{
	MAZErectangle(float l, float b, float w, float h){
		left = l;
		bottom = b;
		width = w;
		height = h;
	}

	MAZErectangle(){
		MAZErectangle(-1, -1, -1, -1);
	}

	float left;
	float bottom;
	float width;
	float height;
};

struct Vec3 {
    Vec3() {
        Vec3(0.0, 0.0, 0.0);
    }
    
    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    float x, y, z;
};

struct Vec2 {
    Vec2() {
        Vec2(0.0, 0.0);
    }
    
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    
    float x, y;
};

/*
 * Colum major matrix for our game
 */
struct MAZEmat{
	MAZEmat(){
		mat[0] = 1;
		mat[1] = 0;
		mat[2] = 0;
		mat[3] = 0;

		mat[4] = 0;
		mat[5] = 1;
		mat[6] = 0;
		mat[7] = 0;

		mat[8] = 0;
		mat[9] = 0;
		mat[10] = 1;
		mat[11] = 0;

		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;
	}
	float mat[16];
};

enum MAZEevent_type{BALL_INTO_PORTAL, BALL_INTO_HOLE};
struct MAZEevent{
	MAZEevent(MAZEevent_type type, void *data){
		ty = type;
		info = data;
	}
	MAZEevent_type ty;
	void *info;
};

void vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz);
void matMultVec3_normalize(struct MAZEmat *matIn, struct Vec3 *vecIn, struct Vec3 *vecOut);

void setVec3(struct Vec3 *vecIn, struct Vec3 *vecOut);
bool Vequal(struct Vec3 *vecIna, struct Vec3 *vecInb);
void VMulti(struct Vec3 *vecIna, float num);
void multMat(struct MAZEmat *mat1, struct MAZEmat *mat2, struct MAZEmat *matout);
struct Vec3 vec2vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz);

void getWindowProjMat(struct MAZErectangle &viewport, struct MAZEmat &projviewMat, struct MAZEmat &viewportMat);
void vecAdd(struct Vec3 *vec1, struct Vec3 *vec2, struct Vec3 *vecOut);
void inverse(struct MAZEmat *matIn, struct MAZEmat *matOut);
void normalize(struct Vec3 *v);
#endif