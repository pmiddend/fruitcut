#include <fruitapp/gui/create_system.hpp>
//#include <fruitapp/gui/dummy/system.hpp>
#include <fruitapp/gui/ce/system.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/cref.hpp>

fruitapp::gui::system_unique_ptr
fruitapp::gui::create_system(
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	sge::charconv::system &_charconv_system,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &_cursor,
	sge::input::mouse::device &_mouse,
	fruitlib::audio::sound_controller &_sound_controller)
{
	return
		fruitapp::gui::system_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::system>(
				fcppt::cref(
					_parent),
				fcppt::ref(
					_renderer),
				fcppt::ref(
					_image_system),
				fcppt::ref(
					_viewport_manager),
				fcppt::ref(
					_charconv_system),
				fcppt::cref(
					_standard_clock_callback),
				fcppt::ref(
					_keyboard),
				fcppt::ref(
					_cursor),
				fcppt::ref(
					_mouse),
				fcppt::ref(
					_sound_controller))
			/*
			fcppt::make_unique_ptr<fruitapp::gui::dummy::system>(
				fcppt::ref(
					_keyboard))*/);
}
