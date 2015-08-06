#version 330 core

out vec2 frag_texcoord;
out vec4 frag_multiply;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texcoord;

layout(location = 2) in vec2 location;
layout(location = 3) in vec4 multiply;

uniform sampler2D texture_major;
uniform mat4 mvp;

void main() {

	frag_texcoord = texcoord.xy;
	frag_multiply = multiply;

	vec3 position_t = position.xyz;

	gl_Position = mvp * vec4(position_t, 1);

}
