#version 120

uniform int index;
uniform float indexScaling;

void main() {
	gl_FragColor = vec4(float(index) / indexScaling,0,0,1);
}
