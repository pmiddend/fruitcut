#if 0
#include <fruitapp/postprocessing/dummy.hpp>

fruitapp::postprocessing::dummy::dummy(
	fruitapp::postprocessing::render_callback const &_render_callback)
:
	render_callback_(
		_render_callback)
{
}

void
fruitapp::postprocessing::dummy::render_result(
	sge::renderer::context::core &_core)
{
	render_callback_(
		_core);
}

fruitapp::postprocessing::dummy::~dummy()
{
}
#endif
