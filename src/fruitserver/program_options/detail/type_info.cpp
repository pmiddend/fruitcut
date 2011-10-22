#include <fcppt/config/external_begin.hpp>
#include <fruitserver/program_options/detail/type_info.hpp>
#include <fcppt/config/external_end.hpp>


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
