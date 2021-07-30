#include "particle.h"

using namespace Particle;
using namespace std;

void base::read(fstream& fs)
{
	//id.read(fs);
}
void base::write(fstream& fs)
{

}
void base::read(char*& buffer)
{

}
void base::write(char*& buffer)
{

}
base::base(int id_) : id(id_)
{

}

void dataNone::f() { }
dataNone::dataNone(int id_) : base(id) { }

void dataBlockState::f() { }
void dataBlockState::read(std::fstream& fs)
{

}
void dataBlockState::write(std::fstream& fs)
{

}
void dataBlockState::read(char*& buffer)
{

}
void dataBlockState::write(char*& buffer)
{

}
dataBlockState::dataBlockState(int id_) : base(id_) { }

void dataColor::f() { }
void dataColor::read(std::fstream& fs)
{

}
void dataColor::write(std::fstream& fs)
{

}
void dataColor::read(char*& buffer)
{

}
void dataColor::write(char*& buffer)
{

}
dataColor::dataColor() : base(15) { }

void dataColorTransition::f() { }
void dataColorTransition::read(std::fstream& fs)
{

}
void dataColorTransition::write(std::fstream& fs)
{

}
void dataColorTransition::read(char*& buffer)
{

}
void dataColorTransition::write(char*& buffer)
{

}
dataColorTransition::dataColorTransition() : base(16) { }

void dataItem::f() { }
void dataItem::read(std::fstream& fs)
{

}
void dataItem::write(std::fstream& fs)
{

}
void dataItem::read(char*& buffer)
{

}
void dataItem::write(char*& buffer)
{

}
dataItem::dataItem() : base(36) { }

void dataVibration::f() { }
void dataVibration::read(std::fstream& fs)
{

}
void dataVibration::write(std::fstream& fs)
{

}
void dataVibration::read(char*& buffer)
{

}
void dataVibration::write(char*& buffer)
{

}
dataVibration::dataVibration() : base(37) { }