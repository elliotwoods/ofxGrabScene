#version 120

uniform int index;

void main() {
	gl_FragColor = vec4(index,0,0,1);
}
