#version 120

uniform sampler2DRect tex;

void main() {
	gl_FragColor = gl_Color * texture2DRect(tex, gl_TexCoord[0].xy);
}
