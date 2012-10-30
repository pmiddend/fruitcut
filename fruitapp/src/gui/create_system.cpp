#include <fruitapp/gui/create_system.hpp>
#include <fruitapp/config.hpp>
#ifdef FRUITAPP_USE_CEGUI
#include <fruitapp/gui/ce/system.hpp>
#else
#include <fruitapp/gui/dummy/system.hpp>
#endif
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/cref.hpp>

fruitapp::gui::system_unique_ptr
fruitapp::gui::create_system(
#ifdef FRUITAPP_USE_CEGUI
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	sge::charconv::system &_charconv_system,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &_cursor,
	fruitlib::audio::sound_controller &_sound_controller,
	sge::renderer::texture::emulate_srgb::type const _emulate_srgb
#else
	fruitlib::scenic::parent const &,
	sge::renderer::device::ffp &,
	sge::image2d::system &,
	sge::viewport::manager &,
	sge::charconv::system &,
	fruitlib::scenic::delta::callback const &,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &,
	fruitlib::audio::sound_controller &
#endif
	)
{
	return
		fruitapp::gui::system_unique_ptr(
#ifdef FRUITAPP_USE_CEGUI
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
					_sound_controller),
				_emulate_srgb)
#else
			fcppt::make_unique_ptr<fruitapp::gui::dummy::system>(
				fcppt::ref(
					_keyboard))
#endif
			);
}
