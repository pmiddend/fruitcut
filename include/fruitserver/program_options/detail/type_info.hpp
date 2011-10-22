#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_TYPE_INFO_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_TYPE_INFO_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <typeinfo>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
namespace program_options
{
namespace detail
{
class type_info
{
public:
	type_info(
		std::type_info const &);

	std::type_info const &
	get() const;
private:
	std::type_info const *info_;
};
}
}
}

#endif
