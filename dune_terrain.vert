#version 330 core

out vec2 frag_texcoord;
out vec3 frag_light_dir;
out vec3 frag_normal;

layout(location = 0) in vec2 position;

// these variables have nothing to do with fragment.
uniform vec2 chunk_position;
// the size of the whole terrain in total (in meters)
uniform vec2 total_size;
// the texcoord of left-top corner point
// of this chunk in the texture cache
uniform vec2 cache_position;
// the size of this chunk (in meters)
uniform vec2 chunk_size;
uniform mat4 model_view_and_projection;

uniform sampler2D heightmap;
uniform sampler2D texcache_diffuse;
uniform sampler2D texcache_normal;

//
// from Tesla@MarXsCube
//
// solution from
//		http://stackoverflow.com/questions/5281261/generating-a-normal-map-from-a-height-map
vec4 parse_heightfield(in sampler2D heightfield_sampler, in vec2 texcoord) {
	const vec2 size = vec2(1.0/24.0, 0.0);
	const vec3 off = vec3(-1.0, 0.0, 1.0) * (1.0/512.0);

	vec4 height = texture(heightfield_sampler, texcoord);

    float s11 = 192 * (height.x);
    float s01 = texture(heightfield_sampler, texcoord+off.xy).x;
    float s21 = texture(heightfield_sampler, texcoord+off.zy).x;
    float s10 = texture(heightfield_sampler, texcoord+off.yx).x;
    float s12 = texture(heightfield_sampler, texcoord+off.yz).x;

    vec3 va = normalize(vec3(size.xy, s21-s01));
    vec3 vb = normalize(vec3(size.yx, s12-s10));

    return vec4(cross(va, vb), s11);
}

void main() {

    // magic: light direction
    vec3 light_dir = vec3(1.5, 1.5, 1);
    frag_light_dir = normalize(light_dir);

    vec2 position_this = position + chunk_position;
    vec2 texcoord_heightmap = vec2(position_this.x / total_size.x, position_this.y / total_size.y);
    vec4 height = parse_heightfield(heightmap, texcoord_heightmap);
    frag_normal = normalize(height.xyz);
    vec4 position_final = vec4(position_this.x, position_this.y, height.w, 1);

    // magic: number of textures inside cache
    frag_texcoord = vec2(cache_position.x + position.x / chunk_size.x / 8,
                        cache_position.y + position.y / chunk_size.y / 8);

	gl_Position = model_view_and_projection * position_final;

}
