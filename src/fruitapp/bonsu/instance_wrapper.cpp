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
#include <sge/texture/part.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assert/pre.hpp>
#include <cmath>

// NOTE: Do NOT call any functions of _instance here because it's not complete!
fruitapp::bonsu::instance_wrapper::instance_wrapper(
	bonsu::instance::base &_instance,
	bonsu::sprite::system &_sprite_system,
	bonsu::texture_manager &_texture_manager,
	bonsu::scalar const _overlay_transparency)
:
	instance_(
		_instance),
	texture_manager_(
		_texture_manager),
	overlay_transparency_(
		_overlay_transparency),
	main_sprite_(
		sprite::parameters()
			// pos, texture, texture_size omitted
			.order(
				0)
			.system(
				&_sprite_system)
			.any_color(
				sge::image::colors::transparent())
			.elements()),
	overlay_sprite_(
		sprite::parameters()
			// pos, texture, texture_size omitted
			.order(
				1)
			.system(
				&_sprite_system)
			.any_color(
				sge::image::colors::transparent())
			.elements()),
	rectangle_manager_(
		0),
	rectangle_instance_(),
	previous_active_state_(
		false)
{
}

void
fruitapp::bonsu::instance_wrapper::update()
{
	instance_.update();

	bool const current_active_state =
		!instance_.dead();


	if(!previous_active_state_ && current_active_state)
	{
		if(rectangle_manager_)
		{
			if(!rectangle_instance_)
			{
				sge::texture::part_ptr const tex =
					texture_manager_.lookup(
						instance_.texture());
				rectangle_instance_.take(
					fcppt::make_unique_ptr<bonsu::rectangle::instance>(
						fcppt::ref(
							*rectangle_manager_),
						fcppt::math::dim::structure_cast<sprite::object::dim>(
							tex->size())));
			}
			else
			{
				rectangle_instance_->revive();
			}
		}
	}
	else if(previous_active_state_ && current_active_state)
	{
		if(rectangle_instance_)
			synchronize();
	}
	else if(previous_active_state_ && !current_active_state)
	{
		FCPPT_ASSERT_PRE(
			!rectangle_instance_ || !(rectangle_instance_->killed()));

		if(rectangle_instance_)
		{
			synchronize();
			rectangle_instance_->kill();
		}
	}
	else
	{
		if(rectangle_instance_)
			synchronize();
	}

	previous_active_state_ =
		current_active_state;
}

fruitapp::bonsu::instance::base &
fruitapp::bonsu::instance_wrapper::instance() const
{
	return instance_;
}

void
fruitapp::bonsu::instance_wrapper::reset_rectangle_manager(
	bonsu::rectangle::manager &_rectangle_manager)
{
	rectangle_manager_ =
		&_rectangle_manager;

	previous_active_state_ =
		false;

	rectangle_instance_.reset();
}

fruitapp::bonsu::instance_wrapper::~instance_wrapper()
{
}

void
fruitapp::bonsu::instance_wrapper::synchronize()
{
	bonsu::scalar const status_fraction =
		1.0f - std::abs(rectangle_instance_->status_fraction());

	sge::texture::part_ptr const tex =
		texture_manager_.lookup(
			instance_.texture());

	main_sprite_.color(
		sprite::color(
			(sge::image::color::init::red %= 1.0)
			(sge::image::color::init::green %= 1.0)
			(sge::image::color::init::blue %= 1.0)
			(sge::image::color::init::alpha %= status_fraction)));

	main_sprite_.texture(
		tex);

	main_sprite_.pos(
		rectangle_instance_->bounds().pos());

	main_sprite_.size(
		fcppt::math::dim::structure_cast<sprite::object::dim>(
			tex->size()));

	overlay_sprite_.color(
		sprite::color(
			(sge::image::color::init::red %= 0.5)
			(sge::image::color::init::green %= 0.5)
			(sge::image::color::init::blue %= 0.5)
			(sge::image::color::init::alpha %= status_fraction * overlay_transparency_)));

	overlay_sprite_.pos(
		rectangle_instance_->bounds().pos());

	overlay_sprite_.size(
		sprite::object::dim(
			main_sprite_.size().w(),
			instance_.progress() * main_sprite_.size().h()));
}
