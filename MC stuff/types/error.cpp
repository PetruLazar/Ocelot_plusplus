#include "error.h"

protocolError::protocolError(const char* msg) : msg(msg) { }
protocolWarning::protocolWarning(const char* msg) : msg(msg) { }
runtimeError::runtimeError(const char* msg) : msg(msg) { }
runtimeWarning::runtimeWarning(const char* msg) : msg(msg) { }
