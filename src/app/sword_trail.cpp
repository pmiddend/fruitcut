#include "sword_trail.hpp"
#include "../fruitlib/cursor_trail.hpp"
#include "../fruitlib/time_format/string_to_duration.hpp"
#include "../fruitlib/json/path.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../media_path.hpp"
#include <sge/sprite/dont_sort.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/time/millisecond.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
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

fruitcut::app::sword_trail::sword_trail(
	sge::renderer::device &_renderer,
	sge::renderer::target_base &_target,
	sge::image2d::multi_loader &_image_loader,
	sge::input::cursor::object &_cursor,
	sge::time::callback const &_time_callback,
	sge::parse::json::object const &_config_file)
:
	cursor_(
		_cursor),
	target_(
		_target),
	time_callback_(
		_time_callback),
	update_interval_(
		*fruitlib::time_format::string_to_duration<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("update-interval")))),
	element_lifetime_(
		*fruitlib::time_format::string_to_duration<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("element-lifetime")))),
	max_width_(
		fruitlib::json::find_and_convert_member<sprite_object::unit>(
			_config_file,
			fruitlib::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("sword-width"))),
	texture_(
		fcppt::make_shared_ptr<sge::texture::part_raw>(
			sge::renderer::texture::create_planar_from_path(
				fruitcut::media_path() / FCPPT_TEXT("textures") / FCPPT_TEXT("sword_particle.png"),
				_renderer,
				_image_loader,
				sge::renderer::texture::filter::linear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::repeat),
				sge::renderer::resource_flags::none))),
	sprite_system_(
		_renderer),
	positions_(
		fruitlib::json::find_and_convert_member<position_buffer::size_type>(
			_config_file,
			fruitlib::json::path(FCPPT_TEXT("sword-mouse-trail")) / FCPPT_TEXT("position-storage-size"))),
	sprites_(
		static_cast<sprite_buffer::size_type>(
			positions_.capacity()-1)),
	timers_(
		static_cast<sprite_buffer::size_type>(
			positions_.capacity()-1)),
	update_timer_(
		update_interval_,
		sge::time::activation_state::active,
		_time_callback)
{
}

fruitcut::app::sword_trail::~sword_trail()
{
}

void
fruitcut::app::sword_trail::update()
{
	for(timer_buffer::size_type i = 0; i < timers_.size(); ++i)
		sprites_[static_cast<sprite_buffer::size_type>(i)].h(
			std::max(
				static_cast<sge::time::funit>(0.0),
				(1.0f - timers_[i]->elapsed_frames()) * max_width_));

	if (!update_timer_.update_b())
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
		fcppt::make_shared_ptr<sge::time::timer>(
			element_lifetime_,
			sge::time::activation_state::active,
			time_callback_,
			sge::time::expiration_state::not_expired));
}

void
fruitcut::app::sword_trail::render()
{
	sprite_system_.render(
		sprites_.begin(),
		sprites_.end(),
		sge::sprite::dont_sort(),
		sge::sprite::default_equal());
}
