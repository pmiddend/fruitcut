#include <fruitapp/postprocessing/dummy/system.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/main.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/paused.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitapp::postprocessing::dummy::system::system()
:
	main_render_callback_()
{
}

fruitapp::postprocessing::subsystems::main_unique_ptr
fruitapp::postprocessing::dummy::system::create_main_subsystem(
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::postprocessing::render_callback const &_render_callback)
{
	main_render_callback_ =
		_render_callback;

	return
		fruitapp::postprocessing::subsystems::main_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::subsystems::main>(
				*this,
				_parent,
				_render_callback));
}

fruitapp::postprocessing::subsystems::paused_unique_ptr
fruitapp::postprocessing::dummy::system::create_paused_subsystem(
	fruitlib::scenic::optional_parent const &_parent)
{
	return
		fruitapp::postprocessing::subsystems::paused_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::subsystems::paused>(
				*this,
				_parent,
				main_render_callback_));
}

fruitapp::postprocessing::dummy::system::~system()
{
}
