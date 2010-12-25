#include "identity.hpp"

fruitcut::pp::filter::identity::identity(
	callback const &_callback)
:
	callback_(
		_callback)
{
}

sge::renderer::texture_ptr const
fruitcut::pp::filter::identity::apply(
	sge::renderer::texture_ptr const t)
{
	callback_();
	return t;
}
