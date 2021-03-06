#include <fruitapp/machine.hpp>
#include <fruitapp/scoped_time_factor.hpp>


fruitapp::scoped_time_factor::scoped_time_factor(
	machine &_m,
	fruitapp::ingame_clock::float_type const _new_factor)
:
	m_(
		_m),
	old_factor_(
		m_.time_factor())
{
	m_.time_factor(
		_new_factor);
}

fruitapp::scoped_time_factor::~scoped_time_factor()
{
	m_.time_factor(
		old_factor_);
}
