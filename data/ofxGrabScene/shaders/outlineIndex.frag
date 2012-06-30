#version 120

uniform sampler2D my_color_texture;
uniform float width = 2.; // measured in texture coordinates
uniform int index = 1;
uniform float valueOffset;

void main(){
	vec4 tex = texture2D(my_color_texture, texture_coordinate);
	
	if (int(tex.r * valueOffset + 0.5) == index)
		gl_FragColor = vec4(1, 0, 0, 1);
	else
		gl_FragColor = (vec4) 0.0;
}