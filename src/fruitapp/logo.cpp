#include <fruitapp/logo.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/center.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/defaults/defaults.hpp>
#include <sge/sprite/process/one.hpp>
#include <sge/texture/part_raw_ptr.hpp>
#include <mizuiro/color/operators.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>


fruitapp::logo::logo(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_loader,
	sge::parse::json::object const &_config_file,
	fruitapp::viewport::manager &_viewport_manager)
:
	node_base(
		_parent),
	sprite_states_(
		_renderer,
		sprite_state_parameters()),
	sprite_buffers_(
		_renderer,
		sge::sprite::buffers::option::dynamic),
	texture_(
		fcppt::make_unique_ptr<sge::texture::part_raw_ptr>(
			sge::renderer::texture::create_planar_from_path(
				fruitlib::media_path()
					/ FCPPT_TEXT("textures")
					/
						sge::parse::json::find_and_convert_member<fcppt::string>(
							_config_file,
							sge::parse::json::path(
								FCPPT_TEXT("textures"))
								/ FCPPT_TEXT("logo")),
				_renderer,
				_image_loader,
				sge::renderer::texture::mipmap::off(),
				sge::renderer::resource_flags_field::null()))),
	sprite_object_(
		sge::sprite::default_parameters<sprite_choices>()
			.texture(
				sprite_object::texture_type(
					*texture_))
			.texture_size()),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::tr1::bind(
				&logo::viewport_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true)))
{
}

fruitapp::logo::~logo()
{
}

void
fruitapp::logo::react(
	fruitlib::scenic::events::render const &_render_event)
{
	sge::sprite::process::one(
		_render_event.context(),
		sprite_object_,
		sprite_buffers_,
		sprite_states_);
}

void
fruitapp::logo::viewport_change(
	sge::renderer::target::viewport const &_viewport)
{
	sge::renderer::pixel_rect::dim const viewport_size(
		_viewport.get().size());

	sge::sprite::center(
		sprite_object_,
		sprite_object::vector(
			static_cast<sprite_object::vector::value_type>(
				viewport_size.w()/2),
			static_cast<sprite_object::vector::value_type>(
				viewport_size.h()/4)));
}
