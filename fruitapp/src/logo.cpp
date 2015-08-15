#include <fruitapp/logo.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/texture_manager.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/process/one.hpp>
#include <sge/sprite/roles/center.hpp>
#include <sge/sprite/roles/texture0.hpp>
#include <sge/texture/part_raw_ptr.hpp>
#include <mizuiro/color/operators.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/unique_ptr_to_const.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::logo::logo(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	fruitlib::texture_manager &_texture_manager,
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
		fcppt::unique_ptr_to_const(
			fcppt::unique_ptr_to_base<
				sge::texture::part
			>(
				fcppt::make_unique_ptr_fcppt<sge::texture::part_raw_ptr>(
					_texture_manager.create_planar_from_path(
						fruitapp::media_path()
							/
								FCPPT_TEXT("textures")
							/
								FCPPT_TEXT("logo.png"),
						sge::renderer::texture::mipmap::off(),
						sge::renderer::resource_flags_field::null()))))),
	sprite_object_(
		sge::sprite::roles::center{} =
			sprite_object::vector::null(),
		sge::sprite::roles::texture0{} =
			sprite_object::texture_type(
				*texture_
			)
	),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			fruitapp::viewport::change_callback{
				std::bind(
					&logo::viewport_change,
					this,
					std::placeholders::_1
				)
			},
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

	sprite_object_.center(
		sprite_object::vector(
			static_cast<sprite_object::vector::value_type>(
				viewport_size.w()/2),
			static_cast<sprite_object::vector::value_type>(
				viewport_size.h()/4)));
}
