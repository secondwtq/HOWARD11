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

vec3 layertex_single(in sampler2D tex, in int idx) {
    if (uvscales[0][idx] > 1e-6) {
        vec3 texcolor1 = texture(tex,
            frag_texcoord * uvscales[0][idx]).rgb;
        if (uvscales[1][idx] > 1e-6) {
            vec3 texcolor2 = texture(tex,
                frag_texcoord * uvscales[1][idx]).rgb;
            if (uvscales[2][idx] > 1e-6) {
                vec3 texcolor3 = texture(tex,
                    frag_texcoord * uvscales[2][idx]).rgb;
                return texcolor1 / 3.0 + texcolor2 / 3.0 + texcolor3 / 3.0;
            } else { return mix(texcolor1, texcolor2, 0.5); }
        } else { return texcolor1; }
    } else { return vec3(0.0, 0.0, 0.0); }
}

vec4 layertex(in sampler2D tex, in sampler2D mask, in int idx) {
    if (uvscales[0][idx] > 1e-6) {
        float alpha = texture(mask, frag_texcoord).r;
        if (alpha > 1e-6) {
            return vec4(layertex_single(tex, idx), alpha);
        } else { return vec4(0.0, 0.0, 0.0, 0.0); }
    } else { return vec4(0.0, 0.0, 0.0, 0.0); }
}

void main() {

    vec4 l1 = layertex(texture1, mask1, 0);
    vec4 l2 = layertex(texture2, mask2, 1);
    vec4 l3 = layertex(texture3, mask3, 2);
    vec4 l4 = layertex(texture4, mask4, 3);

    float alpha_sum = l1.a + l2.a + l3.a + l4.a;
    vec3 final = vec3(0.0, 0.0, 0.0);
    final = mix(final, l1.rgb, l1.a);
    final = mix(final, l2.rgb, l2.a);
    final = mix(final, l3.rgb, l3.a);
    final = mix(final, l4.rgb, l4.a);

    // vec3 final = l1.rgb * (l1.a / alpha_sum) +
    //         l2.rgb * (l2.a / alpha_sum) +
    //         l3.rgb * (l3.a / alpha_sum) +
    //         l4.rgb * (l4.a / alpha_sum);

    color = vec4(final, 1.0);
}
