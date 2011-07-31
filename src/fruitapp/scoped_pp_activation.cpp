#include <fruitapp/scoped_pp_activation.hpp>
#include <fruitapp/postprocessing.hpp>

fruitapp::scoped_pp_activation::scoped_pp_activation(
	fruitapp::postprocessing &_pp,
	bool const new_activation)
:
	pp_(
		_pp),
	old_activation_(
		pp_.active())
{
	pp_.active(
		new_activation);
}

fruitapp::scoped_pp_activation::~scoped_pp_activation()
{
	pp_.active(
		old_activation_);
}
