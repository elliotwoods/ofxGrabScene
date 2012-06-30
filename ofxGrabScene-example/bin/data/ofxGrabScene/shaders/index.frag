#version 120

uniform int index;
uniform float valueOffset;

float toScaled(int index) {
	return float(index) / valueOffset;
}

int toIndex(float scaled) {
	return int((scaled * valueOffset) + 0.5f);
}

void main() {
	gl_FragColor = vec4(toScaled(index), 0, 0, 1.);
}
