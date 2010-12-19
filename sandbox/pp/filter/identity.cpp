#include "identity.hpp"

fruitcut::sandbox::pp::filter::identity::identity(
	callback const &_callback)
:
	callback_(
		_callback)
{
}

sge::renderer::texture_ptr const
fruitcut::sandbox::pp::filter::identity::apply(
	sge::renderer::texture_ptr const t)
{
	callback_();
	return t;
}
