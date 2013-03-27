#include <fruitapp/gui/create_system.hpp>
#include <fruitapp/config.hpp>
#ifdef FRUITAPP_USE_CEGUI
#include <fruitapp/gui/ce/system.hpp>
#else
#include <fruitapp/gui/dummy/system.hpp>
#endif
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitapp::gui::system_unique_ptr
fruitapp::gui::create_system(
#ifdef FRUITAPP_USE_CEGUI
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &_cursor,
	fruitlib::audio::sound_controller &_sound_controller,
	sge::renderer::texture::emulate_srgb const _emulate_srgb
#else
	fruitlib::scenic::parent const &,
	sge::renderer::device::ffp &,
	sge::image2d::system &,
	sge::viewport::manager &,
	fruitlib::scenic::delta::callback const &,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &,
	fruitlib::audio::sound_controller &,
	sge::renderer::texture::emulate_srgb
#endif
	)
{
	return
		fruitapp::gui::system_unique_ptr(
#ifdef FRUITAPP_USE_CEGUI
			fcppt::make_unique_ptr<fruitapp::gui::ce::system>(
				_parent,
				_renderer,
				_image_system,
				_viewport_manager,
				_standard_clock_callback,
				_keyboard,
				_cursor,
				_sound_controller,
				_emulate_srgb)
#else
			fcppt::make_unique_ptr<fruitapp::gui::dummy::system>(
				_keyboard)
#endif
			);
}
