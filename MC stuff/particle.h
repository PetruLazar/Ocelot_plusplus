#pragma once
#include "basic.h"

//unused at the moment
//WIP

namespace Particle
{
	class base
	{
		//pure virtual function to interdict objects of class 'base'
		virtual void f() = 0;

	public:
		varInt id;

		virtual void read(std::fstream&);
		virtual void write(std::fstream&);
		virtual void read(char*&);
		virtual void write(char*&);

		base(int);
	};

	class dataNone : public base
	{
		void f();

	public:
		dataNone(int);
	};
	class dataBlockState : public base
	{
		void f();

	public:
		varInt blockState;

		void read(std::fstream&);
		void write(std::fstream&);
		void read(char*&);
		void write(char*&);

		dataBlockState(int);
	};
	class dataColor : public base
	{
		void f();

	public:
		float reg, green, blue, scale;

		void read(std::fstream&);
		void write(std::fstream&);
		void read(char*&);
		void write(char*&);

		dataColor();
	};
	class dataColorTransition : public base
	{
		void f();

	public:
		float fromRed, fromGreen, fromBlue,
			scale,
			toRed, toGreen, toBlue;

		void read(std::fstream&);
		void write(std::fstream&);
		void read(char*&);
		void write(char*&);

		dataColorTransition();
	};
	class dataItem : public base
	{
		void f();

	public:
		//Slot item;

		void read(std::fstream&);
		void write(std::fstream&);
		void read(char*&);
		void write(char*&);

		dataItem();
	};
	class dataVibration : public base
	{
		void f();

	public:
		double originX, originY, originZ,
			destX, destY, destZ;
		int ticks;

		void read(std::fstream&);
		void write(std::fstream&);
		void read(char*&);
		void write(char*&);

		dataVibration();
	};

	static base* getPofID(int id)
	{
		/*switch (id)
		{
			
		}*/
	}
	static void read()
	{
		
	}
}