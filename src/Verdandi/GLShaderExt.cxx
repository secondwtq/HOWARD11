//
//  GLShaderExt.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/4/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "GLShaderExt.hxx"

#include "Debug.hxx"

#include "GLShader.hxx"
#include "GLShaderExtDef.hxx"

namespace Howard {

namespace Verdandi {

void SpriteShader::init_shader() {
	DEF_ATTRIBUTE_NT(position);
	DEF_ATTRIBUTE_NT(normal);
	DEF_ATTRIBUTE_NT(texcoord);
	DEF_ATTRIBUTE_NT(blendweights);
	DEF_ATTRIBUTE_NT(texindexes);

	DEF_UNIFORM(mvp);
	DEF_SAMPLER(texture_major);
}

void SpriteShader::attribute_attr(size_t sid) {
	switch (sid) {
		case 0:
		{
			SET_ATTRIBUTE3_NTX(position, 0, A, 0);
			SET_ATTRIBUTE3_NTX(texcoord, 1, A, 3);
		}
		break;
		case 1:
		{
			SET_ATTRIBUTE2_NTX(location, 2, A, 0);
			SET_ATTRIBUTE4_NTX(multiply, 3, A, 2);
		}
		break;
		default:
			ASSERT_FOUNDATION();
	}
}

void SpriteShader::disable_attributes() {

	DISABLE_ATTRIBUTE(position);
	DISABLE_ATTRIBUTE(texcoord);
	DISABLE_ATTRIBUTE(location);
	DISABLE_ATTRIBUTE(multiply);

}

}

}