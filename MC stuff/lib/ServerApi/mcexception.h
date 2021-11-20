#pragma once

#include <stdexcept>

class mcexception : public std::exception
{
public:
    enum place { UNSPECIFIED, WORLD, ENTITY };

private:
    const char* file;
    unsigned line;
    place p;

public:
    mcexception(place p, const char* msg, const char* file, unsigned line)
        : std::exception(msg), p(p), file(file), line(line) { }

    const char* getFile() const { return file; }
    int getLine() const { return line; }
    place getPlace() const { return p; }
};

