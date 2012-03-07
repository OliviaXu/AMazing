#ifndef CS248_UTIL_H
#define CS248_UTIL_H

typedef struct Rectangle{
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

#endif