
#version 120

uniform float indexScaling;

uniform sampler2DRect texIndex;
uniform float width = 2.; // measured in texture coordinates

uniform int elementHover = 0;
uniform int nodeSelection = 1;
uniform int nodeHover = 0;

uniform int elementCount = 10;

const int spread = 2;
const int step = 1;

bool isElement(int index) {
	return index < elementCount && index > 0;
}

void main() {
	int centerIndex = int(texture2DRect(texIndex, gl_TexCoord[0].xy).r * indexScaling);
	int centerElementIndex = centerIndex;
	int centerNodeIndex = centerIndex - elementCount;

	int outerIndex;
	int outerNodeIndex;
	int outerElementIndex;

	bool lookup;
	vec2 coord;

	////
	//node selection and hover
	//
	bool centerSelect = centerNodeIndex == nodeSelection;
	bool centerHover = centerNodeIndex == nodeHover;
	//
	float changes = 0.;
	for (int x = -spread; x<= spread; x+=step) {
		for (int y = -spread; y<= spread; y+=step) {
			if (x == y)
				continue;
			
			coord = gl_TexCoord[0].xy + vec2(x, y);

			outerIndex = int(texture2DRect(texIndex, coord).r * indexScaling);
			outerNodeIndex = outerIndex - elementCount;

			//selection
			lookup = outerNodeIndex == nodeSelection;
			if (lookup != centerSelect && nodeSelection != 0 && !isElement(outerIndex) && !isElement(centerIndex))
				changes += 1.;

			//hover
			lookup = outerNodeIndex == nodeHover;
			if (lookup != centerHover && nodeHover != 0 && !isElement(outerIndex) && !isElement(centerIndex))
				changes += 0.5;
		}
	}
	//
	////

	vec4 col = vec4(1.);//
	col.a = changes / (spread * spread * 4 / (step * step));
	changes = 0;

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
			outerIndex = int(texture2DRect(texIndex, coord).r * indexScaling);

			lookup = outerIndex == elementHover;
			if (lookup != centerHover && elementHover > 0 && (isElement(outerIndex) || isElement(centerIndex)))
				changes += 1.;
		}
	}
	//
	/////

	col.a += changes / 4.;

	gl_FragColor = col;
}