#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_HPP_INCLUDED

#include "bad_cast.hpp"
#include "type_info.hpp"
#include "option_holder_base.hpp"
#include <sstream>
#include <typeinfo>
#include <string>

namespace fruitcut
{
namespace server
{
namespace program_options
{
namespace detail
{
template<typename T>
class option_holder
:
	public option_holder_base
{
public:
	explicit
	option_holder(
		T const &_value)
	:
		value_(
			_value)
	{
	}

	T const &
	get() const
	{
		return 
			value_;
	}

	T &
	get()
	{
		return 
			value_;
	}

	detail::type_info const
	type() const
	{
		return 
			typeid(
				T);
	}

	void
	from_string(
		std::string const &s)
	{
		std::stringstream ss;

		if (!(ss << s && ss >> value_))
			throw 
				detail::bad_cast(
					("Couldn't parse \""+
					s+
					"\" to type type "+
					std::string(
						typeid(T).name())).c_str());
	}

	std::string const
	to_string() const
	{
		std::ostringstream ss;
		ss << value_;
		return ss.str();
	}
private:
	T value_;
};
}
}
}
}

#endif
