#include "option_base.hpp"

fruitserver::program_options::detail::option_base::option_base(
	std::string const &_name,
	option_holder_base_ptr const _holder)
:
	name_(
		_name),
	holder_(
		_holder)
{
}

std::string const &
fruitserver::program_options::detail::option_base::name() const
{
	return name_;
}

fruitserver::program_options::detail::option_holder_base_ptr const 
fruitserver::program_options::detail::option_base::holder() const
{
	return holder_;
}
