#include "type_info.hpp"

fruitserver::program_options::detail::type_info::type_info(
	std::type_info const &_info)
:
	info_(
		&_info)
{
}

std::type_info const &
fruitserver::program_options::detail::type_info::get() const
{
	return *info_;
}
