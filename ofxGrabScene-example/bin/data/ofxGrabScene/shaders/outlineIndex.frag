#version 120

uniform sampler2DRect texIndex;
uniform float width = 2.; // measured in texture coordinates

uniform int elementHover = 0;
uniform int nodeSelection = 1;
uniform int nodeHover = 0;

uniform int elementCount = 10;

const int spread = 4;
const int step = 2;

bool isElement(int index) {
	return index < elementCount && index != 0;
}

void main() {
	float changes = 0.;

	int centerElementIndex = int(texture2DRect(texIndex, gl_TexCoord[0].xy).r);
	int centerNodeIndex = centerElementIndex - elementCount;
	if (centerNodeIndex < 0)
		centerNodeIndex = -1;

	int outerIndex;

	bool lookup;
	vec2 coord;

	////
	//node selection and hover
	//
	bool centerSelect = centerNodeIndex == nodeSelection;
	bool centerHover = centerNodeIndex == nodeHover;
	//
	for (int x = -spread; x<= spread; x+=step) {
		for (int y = -spread; y<= spread; y+=step) {
			if (x == y)
				continue;
			
			coord = gl_TexCoord[0].xy + vec2(x, y);
			outerIndex = int(texture2DRect(texIndex, coord).r) - elementCount;
			lookup = outerIndex == nodeSelection;
			if (lookup != centerSelect && nodeSelection != 0 && !isElement(outerIndex) && !isElement(centerNodeIndex))
				changes += 1.;

			lookup = outerIndex == nodeHover;
			if (lookup != centerHover && nodeHover != 0 && !isElement(outerIndex) && !isElement(centerNodeIndex))
				changes += 0.2;
		}
	}
	//
	////

	////
	//element hover
	///
	centerHover = centerElementIndex == elementHover;
	//
	for (int x = -1; x<2; x++) {
		for (int y = -1; y<2; y++) {
			if (x == y)
				continue;

			coord = gl_TexCoord[0].xy + vec2(x, y);
			outerIndex = int(texture2DRect(texIndex, coord).r);

			lookup = outerIndex == elementHover;
			if (lookup != centerHover && outerIndex != 0 && centerElementIndex != 0)
				changes += 2.;
		}
	}
	//
	/////
		

	vec4 col = vec4(1.);//
	col.a = changes / (spread * spread * 4 / (step * step));

	//debug
	// col.rgb = vec3(centerIndex) / 2.;
	// col.a = 1.;

	//passthrough
	//col = texture2DRect(texIndex, gl_TexCoord[0].xy);

	gl_FragColor = col;
}