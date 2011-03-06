#ifndef FRUITCUT_SANDBOX_TERMCOLOR_HPP_INCLUDED
#define FRUITCUT_SANDBOX_TERMCOLOR_HPP_INCLUDED

#include <iostream>

namespace fruitcut
{
namespace sandbox
{

namespace foreground_color
{
enum type
{	
	off = 0,
	black = 30,
	red = 31, 
	green = 32,
	yellow = 33,
	blue = 34,
	magenta = 35,
	cyan = 36,
	white = 37
};
}

namespace background_color
{
enum type
{
	off = 0,
	black = 40,
	red = 41, 
	green = 42,
	yellow = 43,
	blue = 44,
	magenta = 45,
	cyan = 46,
	white = 47
};
}

namespace attribute
{
enum type
{
	off = 0,
	bold = 1,
	underscore = 4,
	blink = 5,
	reverse = 7,
	concealed = 8
};
}

class termcolor
{
public:
	explicit
	termcolor(
		foreground_color::type fg,
		background_color::type bg,
		attribute::type attr);

	explicit
	termcolor();

	foreground_color::type fg();
	background_color::type bg();
	attribute::type attr();

private:
	foreground_color::type fg_;
	background_color::type bg_;
	attribute::type attr_;
};

std::ostream&
operator<<(
	std::ostream&,
	fruitcut::sandbox::termcolor&);
}
}

#endif
