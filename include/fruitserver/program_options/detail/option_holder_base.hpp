#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_HPP_INCLUDED

#include <fruitserver/program_options/detail/type_info.hpp>
#include <string>

namespace fruitserver
{
namespace program_options
{
namespace detail
{
class option_holder_base
{
public:
	virtual ~option_holder_base();

	virtual detail::type_info const
	type() const = 0;

	virtual void
	from_string(
		std::string const &) = 0;

	virtual std::string const
	to_string() const = 0;
protected:
	explicit
	option_holder_base();
};
}
}
}

#endif
