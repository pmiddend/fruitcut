#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_HOLDER_GET_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_HOLDER_GET_HPP_INCLUDED

#include <fruitserver/program_options/detail/bad_cast.hpp>
#include <fruitserver/program_options/detail/option_holder_base.hpp>
#include <fruitserver/program_options/detail/option_holder.hpp>
#include <string>
#include <typeinfo>

namespace fruitserver
{
namespace program_options
{
namespace detail
{
template<typename T>
T const
holder_get(
	detail::option_holder_base const &h)
{
	if(h.type().get() != typeid(T))
		throw 
			detail::bad_cast(
				"Invalid type cast (expected "+
				std::string(
					typeid(T).name())+
				", got "+
				h.type().get().name()+
				")");

	return 
		static_cast<detail::option_holder<T> const &>(h).get();
}
}
}
}

#endif
