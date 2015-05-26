//
//  FSMStream.h
//  MarXsCube
//
//  Created by SeconDatke on 1/24/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_FSMStream_h
#define MarXsCube_FSMStream_h

#include "FSMCommon.hxx"

#include <memory>
#include <vector>

namespace Howard {

namespace FSM {

class FSMBasicStream {
	public:

	virtual void close() = 0;

	virtual void write(const char *src) = 0;

	virtual void writeln(const char *src) = 0;

	virtual ~FSMBasicStream() { };

};

class FSMStdStream : public FSMBasicStream {

	friend void init();

	public:

	virtual void close();

	virtual void write(const char *src);

	virtual void writeln(const char *src);

	virtual bool is_root() { return this->m_isroot; }

	private:

	bool m_isroot = false;

};

class FSMFileStream : public FSMBasicStream {
	public:

	virtual void close();

	virtual void write(const char *src);

	virtual void writeln(const char *src);

	private:

	std::shared_ptr<std::ofstream> m_fs = nullptr;
};

// stackoverflow.com/questions/3165563/flexible-logger-class-using-standard-streams-in-c
// really nice!
class FSMCompositeStream : public FSMBasicStream {
	public:

	virtual void close();

	FSMCompositeStream& add_logger(FSMBasicStream& src);

	FSMCompositeStream& add_deflogger();

	virtual void write(const char *src);

	virtual void writeln(const char *src);

	private:
	std::vector<FSMBasicStream *> m_loggers;
};

}

}

#endif
