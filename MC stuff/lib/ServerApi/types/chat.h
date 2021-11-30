#pragma once

#include "..\json\json_compound.h"
#include "..\json\json_string.h"
#include "..\json\json_boolean.h"
#include "typedefs.h"
#include "apidef.h"

class Chat : public json_compound
{
public:
	class optionalComponent
	{
	protected:
		std::vector<json*> comps;

		optionalComponent();
		optionalComponent(json* jsonComp);

	public:
		SERVER_API optionalComponent& operator|(const optionalComponent& other);

		SERVER_API void addCompsToChat(Chat& chat) const;

		SERVER_API static const optionalComponent empty;
	};

	class color : public optionalComponent
	{
	protected:
		color(const std::string& rawText);

	public:
		SERVER_API color(Byte red, Byte green, Byte blue);
		SERVER_API color(const std::string& red, const std::string& green, const std::string& blue);

		class black : public optionalComponent
		{
		public:
			SERVER_API black();
		};
		class dark_blue : public optionalComponent
		{
		public:
			SERVER_API dark_blue();
		};
		class 	dark_green : public optionalComponent
		{
		public:
			SERVER_API dark_green();
		};
		class 	dark_aqua : public optionalComponent
		{
		public:
			SERVER_API dark_aqua();
		};
		class 	dark_red : public optionalComponent
		{
		public:
			SERVER_API dark_red();
		};
		class 	dark_purple : public optionalComponent
		{
		public:
			SERVER_API dark_purple();
		};
		class 	gold : public optionalComponent
		{
		public:
			SERVER_API gold();
		};
		class 	gray : public optionalComponent
		{
		public:
			SERVER_API gray();
		};
		class 	dark_gray : public optionalComponent
		{
		public:
			SERVER_API dark_gray();
		};
		class 	blue : public optionalComponent
		{
		public:
			SERVER_API blue();
		};
		class 	green : public optionalComponent
		{
		public:
			SERVER_API green();
		};
		class 	aqua : public optionalComponent
		{
		public:
			SERVER_API aqua();
		};
		class 	red : public optionalComponent
		{
		public:
			SERVER_API red();
		};
		class 	light_purple : public optionalComponent
		{
		public:
			SERVER_API light_purple();
		};
		class 	yellow : public optionalComponent
		{
		public:
			SERVER_API yellow();
		};
		class 	white : public optionalComponent
		{
		public:
			SERVER_API white();
		};
		class 	reset : public optionalComponent
		{
		public:
			SERVER_API reset();
		};
	};

	class style
	{
		//private constructor - no objects allowed
		style();

	public:
		class obfuscated : public optionalComponent
		{
		public:
			SERVER_API obfuscated(bool);
		};
		class bold : public optionalComponent
		{
		public:
			SERVER_API bold(bool);
		};
		class strikethrough : public optionalComponent
		{
		public:
			SERVER_API strikethrough(bool);
		};
		class underlined : public optionalComponent
		{
		public:
			SERVER_API underlined(bool);
		};
		class italic : public optionalComponent
		{
		public:
			SERVER_API italic(bool);
		};
	};

	SERVER_API Chat();
	SERVER_API Chat(const char* text, optionalComponent optionals = optionalComponent::empty);
	//SERVER_API Chat(const char* text, color cl);
};

//Chat("Hello world", )