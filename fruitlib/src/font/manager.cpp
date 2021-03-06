#include <fruitlib/texture_manager.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/manager.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/log/context_fwd.hpp>


fruitlib::font::manager::manager(
	fcppt::log::context &_log_context,
	sge::renderer::device::ffp &_renderer,
	sge::renderer::texture::emulate_srgb const _emulate_srgb,
	sge::font::system &_font_system,
	fruitlib::texture_manager &_texture_manager,
	sge::parse::json::object const &_json,
	fruitlib::font::base_path const &_base_path)
:
	renderer_(
		_renderer),
	emulate_srgb_(
		_emulate_srgb),
	cache_(
		fcppt::make_unique_ptr<fruitlib::font::cache>(
			_log_context,
			_font_system,
			_texture_manager.image_system(),
			_renderer,
			_json,
			_base_path))
{
}

sge::renderer::device::ffp &
fruitlib::font::manager::renderer() const
{
	return
		renderer_;
}

sge::renderer::texture::emulate_srgb
fruitlib::font::manager::emulate_srgb() const
{
	return
		emulate_srgb_;
}

fruitlib::font::cache &
fruitlib::font::manager::cache()
{
	return
		*cache_;
}

fruitlib::font::manager::~manager()
{
}
