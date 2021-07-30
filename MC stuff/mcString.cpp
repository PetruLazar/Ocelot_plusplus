#include "mcString.h"
#include "varData.h"

mcString::mcString() : std::string() { }
mcString::mcString(const std::string& v) : std::string(v) { }

void mcString::read(std::fstream& fs)
{
	varInt len;
	len.read(fs);
	char* data = new char[len];
	fs.read(data, len);
	assign(data, len);
	delete[] data;
}
void mcString::write(std::fstream& fs)
{
	varInt len = (int)length();
	len.write(fs);
	fs.write(c_str(), len);
}
void mcString::read(char*& buffer)
{
	varInt len;
	len.read(buffer);
	assign(buffer, len);
	buffer += len;
}
void mcString::write(char*& buffer)
{
	varInt len = (int)length();
	len.write(buffer);
	for (ull i = 0; i < length(); i++) *(buffer++) = operator[](i);
}