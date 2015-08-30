#version 330 core

layout(location = 0) in vec2 position;
layout(location = 0) in vec2 texcoord;

out vec2 frag_texcoord;

uniform mat4 model_view_and_projection;

uniform vec2 brush_center;
uniform vec2 brush_size;

uniform vec2 texcoord_start;
uniform vec2 texcoord_end;

uniform sampler2D brush_texture;
uniform sampler2D brush_mask;

void main() {

}
