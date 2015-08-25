#version 330 core

out vec2 frag_texcoord;

layout(location = 0) in vec2 position;

// these variables have nothing to do with fragment.
uniform vec2 chunk_position;
uniform mat4 model_view_and_projection;

uniform sampler2D heightmap;
uniform sampler2D texcache_diffuse;
uniform sampler2D texcache_normal;

void main() {


	gl_Position = mvp * vec4(position_t, 1);

}
