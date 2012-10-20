#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_HPP_INCLUDED

#include <fruitserver/program_options/detail/bad_cast.hpp>
#include <fruitserver/program_options/detail/option_holder_base.hpp>
#include <fruitserver/program_options/detail/type_info.hpp>
#include <sstream>
#include <string>
#include <typeinfo>


namespace fruitserver
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

#endif
