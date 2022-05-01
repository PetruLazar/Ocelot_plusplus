#include "json_string.h"

json_string::json_string(std::string name, std::string value) : json(json::type::string, name), v(value) {}

std::string json_string::stringValue() const { return '"' + v + '"'; }

std::string& json_string::value() { return v; }
const std::string& json_string::value()  const { return v; }
json_string::operator std::string& () { return v; }
json_string::operator  const std::string& ()  const { return v; }
void json_string::operator=(const std::string& str) { v = str; }