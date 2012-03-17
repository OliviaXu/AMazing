attribute float alphaIn;
attribute float sizeIn;

uniform float widthIn;

varying float alpha;

void main() {
	alpha = alphaIn;

	gl_Position = ftransform();
	gl_PointSize = sizeIn * widthIn / gl_Position.w;
}
