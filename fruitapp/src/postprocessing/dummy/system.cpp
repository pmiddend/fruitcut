#include <fruitapp/postprocessing/dummy/system.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/main.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/paused.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/unique_ptr_to_base.hpp>


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
		fcppt::unique_ptr_to_base<
			fruitapp::postprocessing::subsystems::main
		>(
			fcppt::make_unique_ptr_fcppt<fruitapp::postprocessing::dummy::subsystems::main>(
				*this,
				_parent,
				_render_callback));
}

fruitapp::postprocessing::subsystems::paused_unique_ptr
fruitapp::postprocessing::dummy::system::create_paused_subsystem(
	fruitlib::scenic::optional_parent const &_parent)
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::postprocessing::subsystems::paused
		>(
			fcppt::make_unique_ptr_fcppt<fruitapp::postprocessing::dummy::subsystems::paused>(
				*this,
				_parent,
				main_render_callback_));
}

fruitapp::postprocessing::dummy::system::~system()
{
}
