#version 330 core

in vec2 frag_texcoord;
in vec4 frag_multiply;
out vec4 color;

uniform sampler2D texture_major;
uniform mat4 mvp;

void main() {

	vec4 texcolor = texture(texture_major, frag_texcoord);

	color = vec4(1.0, 1.0, 1.0, 1.0) * frag_multiply;

}