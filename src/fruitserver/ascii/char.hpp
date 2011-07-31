#ifndef FRUITSERVER_ASCII_CHAR_HPP_INCLUDED
#define FRUITSERVER_ASCII_CHAR_HPP_INCLUDED

namespace fruitserver
{
namespace ascii
{
class char_
{
public:
	typedef
	unsigned char
	value_type;

	explicit
	char_()
	:
		value_(
			0)
	{
	}

	explicit
	char_(
		value_type const _value)
	:
		value_(
			_value)
	{
	}

	value_type
	value() const
	{
		return value_;
	}

	void
	value(
		value_type const _value)
	{
		value_ = 
			_value;
	}
private:
	value_type value_;
};
}
}

#endif
