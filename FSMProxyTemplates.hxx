//
//  FSMProxyTemplates.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_FSMProxyTemplates_h
#define MarXsCube_FSMProxyTemplates_h

#include "Common.hxx"

#include "FSMCommon.hxx"
#include "FSMLogger.hxx"

#include <string>

namespace Howard {

namespace FSM {

template<>
inline FSMLoggerProxy& operator<<<LoggerEndline>(FSMLoggerProxy& proxy, const LoggerEndline& src) {
	FSMLoggerProxy& t = proxy.log(HO_ENDLINE);
	proxy.reset();
	return t;
}

template<>
inline FSMLoggerProxy& operator<<<std::string>(FSMLoggerProxy& proxy, const std::string& src) {
	return proxy.log(src.c_str());
}

template<>
inline const std::string& convert_to_string(const unsigned int& src) {
	return std::move(std::to_string(src));
}

template<>
inline const std::string& convert_to_string(const std::size_t& src) {
	return std::move(std::to_string(src));
}

template<>
inline const std::string& convert_to_string(const int& src) {
	return std::move(std::to_string(src));
}

}

}

#endif
