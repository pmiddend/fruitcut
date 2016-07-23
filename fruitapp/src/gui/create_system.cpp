#include <fruitapp/gui/create_system.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/system_unique_ptr.hpp>
#include <fruitapp/config.hpp>
#ifdef FRUITAPP_USE_CEGUI
#include <fruitapp/gui/ce/system.hpp>
#else
#include <fruitapp/gui/dummy/system.hpp>
#endif
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/log/context_fwd.hpp>


fruitapp::gui::system_unique_ptr
fruitapp::gui::create_system(
#ifdef FRUITAPP_USE_CEGUI
	fcppt::log::context &_log_context,
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::focus::object &_focus,
	sge::input::keyboard::device &,
	sge::input::cursor::object &_cursor,
	fruitlib::audio::sound_controller &_sound_controller,
	sge::renderer::texture::emulate_srgb const _emulate_srgb
#else
	fcppt::log::context &,
	fruitlib::scenic::parent const &,
	sge::renderer::device::ffp &,
	sge::image2d::system &,
	sge::viewport::manager &,
	fruitlib::scenic::delta::callback const &,
	sge::input::focus::object &,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &,
	fruitlib::audio::sound_controller &,
	sge::renderer::texture::emulate_srgb
#endif
	)
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::gui::system
		>(
#ifdef FRUITAPP_USE_CEGUI
			fcppt::make_unique_ptr<fruitapp::gui::ce::system>(
				_log_context,
				_parent,
				_renderer,
				_image_system,
				_viewport_manager,
				_standard_clock_callback,
				_focus,
				_cursor,
				_sound_controller,
				_emulate_srgb)
#else
			fcppt::make_unique_ptr<fruitapp::gui::dummy::system>(
				_keyboard)
#endif
			);
}
