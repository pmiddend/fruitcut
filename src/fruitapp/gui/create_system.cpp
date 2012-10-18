#include <fruitapp/gui/create_system.hpp>
#include <fruitapp/gui/dummy/system.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>

fruitapp::gui::system_unique_ptr
fruitapp::gui::create_system(
	fruitlib::scenic::parent const &,
	sge::renderer::device::ffp &,
	sge::image2d::system &,
	sge::viewport::manager &,
	sge::charconv::system &,
	fruitlib::scenic::delta::callback const &,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &,
	fruitlib::audio::sound_controller &)
{
	return
		fruitapp::gui::system_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::dummy::system>(
				fcppt::ref(
					_keyboard)));
}
