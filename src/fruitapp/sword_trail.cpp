#include <fruitapp/sword_trail.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include "../media_path.hpp"
#include <sge/sprite/dont_sort.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/atan2.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

namespace
{
sge::input::cursor::position const
transform_position(
	sge::input::cursor::position const &p,
	sge::renderer::pixel_rect const &viewport)
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

fruitapp::sword_trail::sword_trail(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	sge::renderer::target_base &_target,
	sge::image2d::multi_loader &_image_loader,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
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
		fcppt::make_shared_ptr<sge::texture::part_raw>(
			sge::renderer::texture::create_planar_from_path(
				fruitcut::media_path() / FCPPT_TEXT("textures") / FCPPT_TEXT("sword_particle.png"),
				_renderer,
				_image_loader,
				sge::renderer::texture::mipmap::off(),
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::repeat),
				sge::renderer::resource_flags::none))),
	sprite_system_(
		_renderer),
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

fruitapp::sword_trail::~sword_trail()
{
}

void
fruitapp::sword_trail::react(
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

	sprite_object::unit const epsilon = 
		0.0001f;

	sprite_object::vector const new_vector = 
		fcppt::math::vector::structure_cast<sprite_object::vector>(
			transform_position(
				cursor_.position(),
				target_.viewport().get()));

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
			sprite_parameters()
				.center(
					middle)
				.size(
					sprite_object::dim(
						fcppt::math::vector::length(
							diff),
						max_width_))
				.texture(
					texture_)
				.rotation(
					angle)
				.elements()));

	timers_.push_back(
		fcppt::make_shared_ptr<fruitapp::ingame_timer>(
			fruitapp::ingame_timer::parameters(
				clock_,
				element_lifetime_)));
}

void
fruitapp::sword_trail::react(
		fruitlib::scenic::events::render const &)
{
	sprite_system_.render(
		sprites_.begin(),
		sprites_.end(),
		sge::sprite::dont_sort(),
		sge::sprite::default_equal());
}
