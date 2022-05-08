#pragma once

#include <stdexcept>
#include <string>

#include "debug.h"

#include "../types/apidef.h"

#define runtimeError(msg) mcException(msg, "Runtime Error", __FILENAME__, __LINE__)
#define runtimeWarning(msg) mcException(msg, "Runtime Warning", __FILENAME__, __LINE__)
#define protocolError(msg) mcException(msg, "Protocol Error", __FILENAME__, __LINE__)
#define protocolWarning(msg) mcException(msg, "Protocol Warning", __FILENAME__, __LINE__)
#define genException(msg) mcException(msg, "Exception", __FILENAME__, __LINE__)

class mcException : public std::exception
{
private:
    const std::string& file;
    const std::string& type;
    unsigned line;

public:
    mcException(const std::string& msg, const std::string& type, const std::string& file, unsigned line)
        : std::exception(std::string(type + " in '" + file + "' at line " + std::to_string(line) + ": " + msg).c_str()), type(type), file(file), line(line) { }

    const std::string& getFile() const { return file; }
    const std::string& getType() const { return type; }
    const int getLine() const { return line; }
};
