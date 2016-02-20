#include <fruitapp/postprocessing/dummy/subsystems/main.hpp>


fruitapp::postprocessing::dummy::subsystems::main::main(
	fruitapp::postprocessing::render_callback const &_render_callback)
:
	render_callback_(
		_render_callback)
{
}

void
fruitapp::postprocessing::dummy::subsystems::main::render_result(
	sge::renderer::context::core &_context)
{
	render_callback_(
		_context);
}

void
fruitapp::postprocessing::dummy::subsystems::main::loading_progress(
	sge::renderer::scalar)
{
}

fruitapp::postprocessing::dummy::subsystems::main::~main()
{
}
