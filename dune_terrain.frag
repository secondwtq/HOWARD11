#version 330 core

in vec2 frag_texcoord;
out vec4 color;

uniform sampler2D texcache_diffuse;
uniform sampler2D texcache_normal;

void main() {

	color = rgba(1.0, 1.0, 1.0, 0.0);

}
