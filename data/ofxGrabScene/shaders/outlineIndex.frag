#version 120

uniform sampler2DRect texIndex;
uniform float width = 2.; // measured in texture coordinates
uniform int selection = 1;
uniform float valueOffset = 1 << 10;

float toScaled(int index) {
	return float(index) / valueOffset;
}

int toIndex(float scaled) {
	return int((scaled * valueOffset) + 0.5f);
}

const int spread = 4;
const int step = 2;
void main() {
	int changes = 0;
	int centerIndex = toIndex(texture2DRect(texIndex, gl_TexCoord[0].xy).r);
	bool centerSelect = centerIndex == selection;
	bool lookup;
	vec2 coord;
	for (int x = -spread; x<= spread; x+=step) {
		for (int y = -spread; y<= spread; y+=step) {
			if (x == y)
				continue;
			else {
				coord = gl_TexCoord[0].xy + vec2(x, y);
				lookup = toIndex(texture2DRect(texIndex, coord).r) == selection;
				if (lookup != centerSelect)
					changes++;
			}
		}
	}
	vec4 col = vec4(1.);//
	col.a = selection != 0 ? float(changes) / (spread * spread * 4 / (step * step)) : 0.;

	//debug
	// col.rgb = vec3(centerIndex) / 2.;
	// col.a = 1.;

	//passthrough
	//col = texture2DRect(texIndex, gl_TexCoord[0].xy);

	gl_FragColor = col;
}