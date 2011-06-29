#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_OPTION_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_OPTION_HPP_INCLUDED

#include "detail/option_base.hpp"
#include "detail/option_holder.hpp"
#include "detail/option_holder_base_ptr.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
namespace program_options
{
template<typename T>
detail::option_base const
option(
	std::string const &name,
	T const &value)
{
	return 
		detail::option_base(
			name,
			detail::option_holder_base_ptr(
				// bummer, no make_shared available
				new detail::option_holder<T>(
					value)));
}
}
}
}

#endif
