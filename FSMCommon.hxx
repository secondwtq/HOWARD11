//
//  FSMCommon.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_FSMCommon_h
#define MarXsCube_FSMCommon_h

namespace Howard {

enum FSMLevel {
	Highest,
	Fatal,
	Error,
	Warning,
	Debug,
	Message,
	Trace,
	Lowest,
};

namespace FSM {

extern const char *FSMLevelNames[];

struct LoggerEndline { };

}

}

#endif
