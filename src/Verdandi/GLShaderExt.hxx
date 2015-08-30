//
//  GLShaderExt.h
//  MarXsCube
//
//  Created by SeconDatke on 2/4/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GLShaderExt__
#define __MarXsCube__GLShaderExt__

// #include "Platform.h"
#include "GLShader.hxx"

#ifdef CUBE_PLATFORM_DARWIN
#include <OpenGL/gltypes.h>
#else
typedef unsigned int GLuint; // hack, it seems other OS have no gltypes.h
#endif

#define NAME_ATTRIBUTE(name) attr_##name
#define NAME_UNIFORM(name) uni_##name

#define _DEF_ATTRIBUTE(name) GLuint NAME_ATTRIBUTE(name)

#define _DEF_UNIFORM(name) GLuint NAME_UNIFORM(name)
#define _DEF_SAMPLER(name) _DEF_UNIFORM(name)

#define DEF_ATTRIBUTE(name) _DEF_ATTRIBUTE(name)
#define DEF_UNIFORM(name) _DEF_UNIFORM(name)
#define DEF_SAMPLER(name) _DEF_SAMPLER(name)

#define ATTR_OBJECT(id, type) \
	typedef type type_vert_obj_##id; \
	void attribute_attr_##id()

namespace Howard {
namespace Verdandi {

class gl_shader_ext : public gl_shader {
	public:

	virtual void init_shader() { }

	virtual void attribute_attr(size_t sid) { }

	virtual void disable_attributes() { }

};

}
}

#define SET_UNIFORMB(shader, name, value) (glUniform1i((shader).NAME_UNIFORM(name), static_cast<bool>(value)))

#define SET_UNIFORMBP(shader, name, value) SET_UNIFORMB(*(shader), name, (value))

#define SET_UNIFORM2(shader, name, glmv) (glUniform2f((shader).NAME_UNIFORM(name), (glmv).x, (glmv).y))

#define SET_UNIFORM3(shader, name, glmv) (glUniform3f((shader).NAME_UNIFORM(name), (glmv).x, (glmv).y, (glmv).z))

#define SET_UNIFORM4(shader, name, glmv) (glUniform4f((shader).NAME_UNIFORM(name), (glmv).x, (glmv).y, (glmv).z, (glmv).w))

#define SET_UNIFORM2P(shader, name, glmv) (SET_UNIFORM2(*shader, name, glmv))
#define SET_UNIFORM3P(shader, name, glmv) (SET_UNIFORM3(*shader, name, glmv))

#define SET_UNIFORMAT4(shader, name, glmm) (glUniformMatrix4fv((shader).NAME_UNIFORM(name), 1, GL_FALSE, &((glmm)[0][0])))

#define SET_UNIFORMAT4P(shader, name, glmm) (glUniformMatrix4fv((shader)->NAME_UNIFORM(name), 1, GL_FALSE, &((glmm)[0][0])))

#define BIND_TEXTURE(shader, name, texid, texunitid) (glActiveTexture(GL_TEXTURE##texunitid), glBindTexture(GL_TEXTURE_2D, (texid)), glUniform1i(shader.NAME_UNIFORM(name), (texunitid)))

#define BIND_TEXTUREP(shaderp, name, texid, texunitid) (glActiveTexture(GL_TEXTURE##texunitid), glBindTexture(GL_TEXTURE_2D, (texid)), glUniform1i(shaderp->NAME_UNIFORM(name), (texunitid)))


#endif /* defined(__MarXsCube__GLShaderExt__) */
