#include "termcolor.hpp"

namespace fruitcut
{
namespace sandbox
{
termcolor::termcolor(
	foreground_color::type const _fg,
	background_color::type const _bg,
	attribute::type const _attr
	)
{
	fg_ = _fg;
	bg_ = _bg;
	attr_ = _attr;
}

termcolor::termcolor()
{
	fg_ = foreground_color::off;
	bg_ = background_color::off;
	attr_ = attribute::off;
}

foreground_color::type
termcolor::fg()
{
	return fg_;
}

background_color::type
termcolor::bg()
{
	return bg_;
}

attribute::type
termcolor::attr()
{
	return attr_;
}

std::ostream&
operator<<(
	std::ostream &s,
	termcolor &c)
{
	return s 
		<< "\033["
		<< c.attr() << ";"
		<< c.fg() << ";"
		<< c.bg() << "m";
}

}
}

int main()
{
	fruitcut::sandbox::termcolor red_on_blue(
		fruitcut::sandbox::foreground_color::red,
		fruitcut::sandbox::background_color::blue,
		fruitcut::sandbox::attribute::off
	);
	fruitcut::sandbox::termcolor bold_yellow(
		fruitcut::sandbox::foreground_color::yellow,
		fruitcut::sandbox::background_color::black,
		fruitcut::sandbox::attribute::bold
	);
	fruitcut::sandbox::termcolor clear;

	std::cout <<
		bold_yellow <<
		"this should be bold yellow" <<
		clear <<
		std::endl <<
		red_on_blue <<
		"this should be red on blue" <<
		clear <<
		std::endl;
}
