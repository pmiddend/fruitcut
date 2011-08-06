#include <fruitapp/bonsu/instance_wrapper.hpp>
#include <fruitapp/bonsu/instance/base.hpp>
#include <fruitapp/bonsu/texture_manager.hpp>
#include <fruitapp/bonsu/sprite/parameters.hpp>
#include <fruitapp/bonsu/rectangle/instance.hpp>
#include <fruitapp/bonsu/sprite/color_format.hpp>
#include <fruitapp/bonsu/sprite/color.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assert.hpp>
#include <cmath>

fruitapp::bonsu::instance_wrapper::instance_wrapper(
	bonsu::instance::base &_instance,
	bonsu::sprite::system &_sprite_system,
	bonsu::texture_manager &_texture_manager,
	bonsu::rectangle::manager &_rectangle_manager)
:
	instance_(
		_instance),
	main_sprite_(
		sprite::parameters()
			// pos omitted
			.texture(
				_texture_manager.lookup(
					_instance.texture()))
			.texture_size()
			.order(
				0)
			.system(
				&_sprite_system)
			.any_color(
				sge::image::colors::transparent())
			.elements()),
	overlay_sprite_(
		sprite::parameters()
			// pos omitted
			.texture(
				sge::texture::part_ptr())
			.texture_size()
			.order(
				1)
			.system(
				&_sprite_system)
			.any_color(
				sge::image::colors::transparent())
			.elements()),
	rectangle_manager_(
		_rectangle_manager),
	rectangle_instance_(),
	previous_active_state_(
		false)
{
}

void
fruitapp::bonsu::instance_wrapper::update()
{
	bool const current_active_state = 
		!instance_.dead();

	if(!previous_active_state_ && current_active_state)
	{
		if(!rectangle_instance_)
			rectangle_instance_.take(
				fcppt::make_unique_ptr<bonsu::rectangle::instance>(
					fcppt::ref(
						rectangle_manager_),
					main_sprite_.size()));
		else
			rectangle_instance_->revive();
	}
	else if(previous_active_state_ && current_active_state)
	{
		FCPPT_ASSERT(
			rectangle_instance_);

		synchronize();
	}
	else if(previous_active_state_ && !current_active_state)
	{
		FCPPT_ASSERT(
			rectangle_instance_);

		FCPPT_ASSERT(
			!rectangle_instance_->killed());

		synchronize();

		rectangle_instance_->kill();
	}
	else
	{
		FCPPT_ASSERT(
			rectangle_instance_);

		synchronize();
	}

	previous_active_state_ = 
		current_active_state;

	// - if viewport was reset, reset all rectangle instances
}

fruitapp::bonsu::instance_wrapper::~instance_wrapper()
{
}

void
fruitapp::bonsu::instance_wrapper::synchronize()
{
	FCPPT_ASSERT(
		rectangle_instance_);

	sprite::color const new_color(
		(sge::image::color::init::red %= 1.0)
		(sge::image::color::init::green %= 1.0)
		(sge::image::color::init::blue %= 1.0)
		(sge::image::color::init::alpha %= 1.0f - std::abs(rectangle_instance_->status_fraction())));
	main_sprite_.color(
		new_color);
	overlay_sprite_.color(
		new_color);
	main_sprite_.pos(
		rectangle_instance_->bounds().pos());
	overlay_sprite_.pos(
		rectangle_instance_->bounds().pos());
	overlay_sprite_.size(
		sprite::object::dim(
			overlay_sprite_.size().w(),
			instance_.progress() * overlay_sprite_.size().h()));
}
