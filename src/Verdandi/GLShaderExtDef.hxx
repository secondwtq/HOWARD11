//
//  GLShaderExtDef.h
//  MarXsCube
//
//  Created by SeconDatke on 2/11/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_GLShaderExtDef_h
#define MarXsCube_GLShaderExtDef_h

#include "GLCommon.hxx"
#include <type_traits>

#undef DEF_ATTRIBUTE
#undef DEF_UNIFORM
#undef DEF_SAMPLER

#define DEF_ATTRIBUTE(name) ((this->NAME_ATTRIBUTE(name)) = (this->attribute(#name)))
#define DEF_UNIFORM(name) ((this->NAME_UNIFORM(name)) = (this->uniform(#name)))
#define DEF_SAMPLER(name) DEF_UNIFORM(name)

#define DEF_ATTRIBUTE_NT(name)

#define SET_ATTRIBUTE3(name, pos) (glVertexAttribPointer(this->NAME_ATTRIBUTE(name), 3, GL_FLOAT, GL_FALSE, sizeof(std::remove_reference<decltype(*this)>::type::type_vert_obj), (char *)(pos*sizeof(GLfloat))), glEnableVertexAttribArray(this->NAME_ATTRIBUTE(name)))

#define SET_ATTRIBUTE3_NT(name, loc, pos) (glEnableVertexAttribArray((loc)), glVertexAttribPointer((loc), 3, GL_FLOAT, GL_FALSE, sizeof(std::remove_reference<decltype(*this)>::type::type_vert_obj), (char *)(pos*sizeof(GLfloat))))

#define SET_ATTRIBUTE_NTX(name, loc, obj, pos, slots) (glEnableVertexAttribArray((loc)), \
    glVertexAttribPointer((loc), slots, GL_FLOAT, GL_FALSE, sizeof(std::remove_reference<decltype(*this)> \
    ::type::type_vert_obj_##obj), (char *)(pos*sizeof(GLfloat))))

#define SET_ATTRIBUTE3_NTX(name, loc, obj, pos) (SET_ATTRIBUTE_NTX(name, loc, obj, pos, 3))
#define SET_ATTRIBUTE2_NTX(name, loc, obj, pos) (SET_ATTRIBUTE_NTX(name, loc, obj, pos, 2))
#define SET_ATTRIBUTE4_NTX(name, loc, obj, pos) (SET_ATTRIBUTE_NTX(name, loc, obj, pos, 4))

#define DISABLE_ATTRIBUTE(name) (glDisableVertexAttribArray(this->NAME_ATTRIBUTE(name)))

#define DISABLE_ATTRIBUTE_NT(name, loc) (glDisableVertexAttribArray((loc)))

#endif
