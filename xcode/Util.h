#ifndef CS248_UTIL_H
#define CS248_UTIL_H

struct Rectangle{
	Rectangle(float l, float t, float w, float h){
		left = l;
		top = t;
		width = w;
		height = h;
	}

	Rectangle(){
		Rectangle(-1, -1, -1, -1);
	}

	float left;
	float top;
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

void vecAdd(struct Vec3 *vecIn, float dx, float dy, float dz);
#endif