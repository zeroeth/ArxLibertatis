
#include "io/log/ConsoleLogger.h"

#include <cstring>
#include <iostream>

#include "Configure.h"

#if defined(HAVE_ISATTY) || defined(HAVE_READLINK)
#include <unistd.h>
#include <errno.h>
#endif

#include "io/log/ColorLogger.h"

namespace logger {

Console::~Console() {
	// nothing to clean up
}

void Console::log(const Source & file, int line, Logger::LogLevel level, const std::string & str) {
	std::ostream & os = (level == Logger::Warning || level == Logger::Error) ? std::cerr : std::cout;
	format(os, file, line, level, str);
}

void Console::flush() {
	std::cout.flush(), std::cerr.flush();
}

static bool is_fd_disabled(int fd) {
	
	ARX_UNUSED(fd);
	
	// Disable the console log backend if output is redirected to /dev/null
#ifdef HAVE_READLINK
	static const char * names[] = { NULL, "/proc/self/fd/1", "/proc/self/fd/2" };
	char path[64];
	ssize_t len = readlink(names[fd], path, ARRAY_SIZE(path));
	if(len == 9 && !memcmp(path, "/dev/null", 9)) {
		return true;
	} else if(len == -1 && errno == ENOENT) {
		return true;
	}
#endif
	
	return false;
}

Backend * Console::get() {
	
#ifdef HAVE_ISATTY
	if(isatty(1) && isatty(2)) {
		return new ColorConsole;
	}
#endif
	
	if(is_fd_disabled(1) && is_fd_disabled(2)) {
		return NULL;
	}
	
	return new Console;
}

} // namespace logger