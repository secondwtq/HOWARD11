//
//  FSMHelper.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_FSMHelper_h
#define MarXsCube_FSMHelper_h

#include "FSMCommon.hxx"
#include "FSMLogger.hxx"

namespace Howard {

namespace FSMHelper {

using L = FSMLevel;

extern FSM::LoggerEndline rn;

// HOWARD addition of interface
inline FSM::FSMLoggerProxy& log(FSMLevel level) { return FSM::logger(level); }
inline FSM::FSMLogger& log(const std::string& name) { return FSM::logger(name); }
inline FSM::FSMLoggerProxy& log(const std::string& name, FSMLevel level) {
    return FSM::logger(name, level); }

}

}

#endif
