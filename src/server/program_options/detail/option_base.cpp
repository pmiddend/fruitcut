#include "option_base.hpp"

fruitcut::server::program_options::detail::option_base::option_base(
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
fruitcut::server::program_options::detail::option_base::name() const
{
	return name_;
}

fruitcut::server::program_options::detail::option_holder_base_ptr const 
fruitcut::server::program_options::detail::option_base::holder() const
{
	return holder_;
}
