//
//  GLShader.h
//  MarXsCube
//
//  Created by SeconDatke on 1/28/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GLShader__
#define __MarXsCube__GLShader__

#include <string>

namespace Howard {

namespace Verdandi {

enum SHADERTYPE {
	VERTEX, FRAG, PROGRAM };

class gl_shader {
	public:

	void load_file(SHADERTYPE type, const std::string& path);

	void load_str(SHADERTYPE type, const char *src);

	void load_str(SHADERTYPE type, const std::string& src);

	std::string log(SHADERTYPE type);

	void create();

	int attribute(const char *name);

	int uniform(const char *name);

	void use();

    void destroy();

	virtual ~gl_shader() { }

	private:

	unsigned int obj_id = 0;
	unsigned int vert_id = 0, frag_id = 0;
};

}

}

#endif /* defined(__MarXsCube__GLShader__) */
