#version 120
uniform sampler2D diffuseMap;
varying float alpha;

void main() {
	vec4 col = texture2D(diffuseMap, gl_PointCoord);
	col.a = alpha * col.a;
	gl_FragColor = col;
}
