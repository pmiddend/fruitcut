#include <fruitapp/media_path.hpp>
#include <fruitapp/cursor/sword_trail.hpp>
#include <fruitlib/texture_manager.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/geometry/make_random_access_range.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/sprite/roles/center.hpp>
#include <sge/sprite/roles/rotation.hpp>
#include <sge/sprite/roles/size.hpp>
#include <sge/sprite/roles/texture0.hpp>
#include <sge/texture/part_raw_ptr.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/unique_ptr_to_const.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/vector/atan2.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/optional/maybe_void.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <fcppt/config/external_end.hpp>


namespace
{
sge::input::cursor::position const
transform_position(
	sge::input::cursor::position const &p,
	sge::renderer::pixel_rect const &)
{
	return p;
/*
	return
		sge::input::cursor::position(
			static_cast<sge::input::cursor::position_unit>(
				p.x() - viewport.pos().x()),
			static_cast<sge::input::cursor::position_unit>(
				viewport.size().h() - p.y() + viewport.pos().y()));
*/
}
}

fruitapp::cursor::sword_trail::sword_trail(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	sge::renderer::device::ffp &_renderer,
	sge::renderer::target::base const &_target,
	fruitlib::texture_manager &_texture_manager,
	sge::parse::json::object const &_config_file)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	target_(
		_target),
	clock_(
		_clock),
	element_lifetime_(
		fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_clock::duration>(
			_config_file,
			sge::parse::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("element-lifetime"))),
	max_width_(
		sge::parse::json::find_and_convert_member<sprite_object::unit>(
			_config_file,
			sge::parse::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("sword-width"))),
	texture_(
		fcppt::unique_ptr_to_const(
			fcppt::unique_ptr_to_base<
				sge::texture::part
			>(
				fcppt::make_unique_ptr<sge::texture::part_raw_ptr>(
					_texture_manager.create_planar_from_path(
						fruitapp::media_path() / FCPPT_TEXT("textures") / FCPPT_TEXT("sword_particle.png"),
						sge::renderer::texture::mipmap::off(),
						sge::renderer::resource_flags_field::null()))))),
	sprite_buffers_(
		_renderer,
		sge::sprite::buffers::option::dynamic),
	sprite_states_(
		_renderer,
		sprite_state_parameters()),
	positions_(
		sge::parse::json::find_and_convert_member<position_buffer::size_type>(
			_config_file,
			sge::parse::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("position-storage-size"))),
	sprites_(
		static_cast<sprite_buffer::size_type>(
			positions_.capacity()-1)),
	timers_(
		static_cast<sprite_buffer::size_type>(
			positions_.capacity()-1)),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			clock_,
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_clock::duration>(
				_config_file,
				sge::parse::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("update-interval"))))
{
}

fruitapp::cursor::sword_trail::~sword_trail()
{
}

void
fruitapp::cursor::sword_trail::react(
	fruitlib::scenic::events::update const &)
{
	for(timer_buffer::size_type i = 0; i < timers_.size(); ++i)
		sprites_[static_cast<sprite_buffer::size_type>(i)].h(
			std::max(
				static_cast<sprite_object::unit>(
					0),
				(1.0f - sge::timer::elapsed_fractional<sprite_object::unit>(*timers_[i])) * max_width_));

	if (!sge::timer::reset_when_expired(update_timer_))
		return;

	fcppt::optional::maybe_void(
		cursor_.position(),
		[
			this
		](
			sge::input::cursor::position const _position
		)
		{
			sprite_object::vector const new_vector =
				fcppt::math::vector::structure_cast<sprite_object::vector, fcppt::cast::size_fun>(
					transform_position(
						_position,
						target_.viewport().get()));

			sprite_object::unit const epsilon =
				0.0001f;

			if(!positions_.empty() && fcppt::math::vector::length(new_vector - positions_.back()) < epsilon)
				return;

			positions_.push_back(
				new_vector);

			if(positions_.size() == 1u)
				return;

			sprite_object::vector const
				diff =
					(*boost::prior(positions_.end())) - (*boost::prior(positions_.end(),2)),
				middle =
					((*boost::prior(positions_.end())) + (*boost::prior(positions_.end(),2)))
						/ static_cast<sprite_object::unit>(2);

			sprite_object::unit const angle =
				fcppt::math::vector::atan2(
					diff);

			sprites_.push_back(
				sprite_object(
					sge::sprite::roles::center{} =
						middle,
					sge::sprite::roles::size{} =
						sprite_object::dim(
							fcppt::math::vector::length(
								diff
							),
							max_width_
						),
					sge::sprite::roles::texture0{} =
						sprite_object::texture_type(
							*texture_
						),
					sge::sprite::roles::rotation{} =
						angle
				)
			);

			timers_.push_back(
				fcppt::make_shared_ptr<fruitapp::ingame_timer>(
					fruitapp::ingame_timer::parameters(
						clock_,
						element_lifetime_)));
		}
	);
}

void
fruitapp::cursor::sword_trail::react(
	fruitlib::scenic::events::render const &_render_event)
{
	sge::sprite::process::all(
		_render_event.context(),
		sge::sprite::geometry::make_random_access_range(
			sprites_),
		sprite_buffers_,
		sge::sprite::compare::default_(),
		sprite_states_);
}
