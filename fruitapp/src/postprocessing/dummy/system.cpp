#include <fruitapp/postprocessing/dummy/system.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/main.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/paused.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/unique_ptr_to_base.hpp>


fruitapp::postprocessing::dummy::system::system()
{
}

fruitapp::postprocessing::subsystems::main_unique_ptr
fruitapp::postprocessing::dummy::system::create_main_subsystem(
	fruitlib::scenic::optional_parent const &,
	fruitapp::postprocessing::render_callback const &_render_callback)
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::postprocessing::subsystems::main
		>(
			fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::subsystems::main>(
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
			fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::subsystems::paused>(
				_parent));
}

fruitapp::postprocessing::dummy::system::~system()
{
}
