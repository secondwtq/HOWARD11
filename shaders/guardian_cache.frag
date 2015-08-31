#version 330 core

in vec2 frag_texcoord;

out vec4 color;

//uniform vec4 color_multiply;

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

    vec3 final = vec3(0, 0, 0);

    if (uvscales[0][0] > 1e-6) {
        vec3 texcolor = texture(texture1, frag_texcoord * uvscales[0][0]).rgb;
        float alpha = texture(mask1, frag_texcoord).r;
        final = mix(final, texcolor, alpha);
    }
    if (uvscales[0][1] > 1e-6) {
        vec3 texcolor = texture(texture2, frag_texcoord * uvscales[0][1]).rgb;
        float alpha = texture(mask2, frag_texcoord).r;
        final = mix(final, texcolor, alpha);
    }
    if (uvscales[0][2] > 1e-6) {
        vec3 texcolor = texture(texture3, frag_texcoord * uvscales[0][2]).rgb;
        float alpha = texture(mask3, frag_texcoord).r;
        final = mix(texcolor, final, alpha);
    }
    if (uvscales[0][3] > 1e-6) {
        vec3 texcolor = texture(texture4, frag_texcoord * uvscales[0][3]).rgb;
        float alpha = texture(mask4, frag_texcoord).r;
        final = mix(texcolor, final, alpha);
    }

    color = vec4(final, 1.0);
}
