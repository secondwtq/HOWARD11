#version 330 core

in vec2 frag_texcoord;
in vec3 frag_light_dir;
in vec3 frag_normal;

out vec4 color;

uniform sampler2D texcache_diffuse;
uniform sampler2D texcache_normal;

void main() {

    float light_intensity = dot(frag_light_dir, frag_normal);

    vec3 diffuse = texture(texcache_diffuse, frag_texcoord).rgb;

    // magic: overbright factor
    vec3 ret = diffuse * light_intensity * 1.2;
    color = vec4(ret, 1.0);
}
