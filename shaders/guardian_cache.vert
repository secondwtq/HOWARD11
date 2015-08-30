#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 frag_texcoord;

uniform mat4 model_view_and_projection;

uniform vec2 brush_center;
//uniform vec2 brush_size;

//uniform vec2 texcoord_start;
//uniform vec2 texcoord_end;

uniform mat4 uvscales;

uniform sampler2D texture1;
uniform sampler2D mask1;

uniform sampler2D texture2;
uniform sampler2D mask2;

uniform sampler2D texture3;
uniform sampler2D mask3;

uniform sampler2D texture4;
uniform sampler2D mask4;

void main() {

    frag_texcoord = texcoord.xy;

    vec2 position_t = vec2(brush_center.x + position.x * 256, brush_center.y + position.y * 256);
    vec4 position_4 = vec4(position_t.xy, 0, 1);

    gl_Position = model_view_and_projection * position_4;
}
