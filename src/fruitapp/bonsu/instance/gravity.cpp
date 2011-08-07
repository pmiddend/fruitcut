#include <fruitapp/bonsu/instance/gravity.hpp>
#include <fruitlib/json/find_and_convert_member.hpp> 
#include <fruitlib/time_format/find_and_convert_duration.hpp> 
#include <fruitlib/json/path.hpp> 
#include <fruitlib/physics/vector3.hpp> 
#include <fruitlib/physics/world.hpp> 
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <fcppt/text.hpp>

fruitapp::bonsu::instance::gravity::gravity(
	bonsu::manager &_bonsu_manager,
	sge::parse::json::object const &_config_file,
	fruitlib::physics::world &_world,
	fruitapp::ingame_clock const &_clock)
:
	instance::base(
		_bonsu_manager),
	world_(
		_world),
	default_gravity_(
		fruitlib::json::find_and_convert_member<fruitlib::physics::vector3>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("bonsu")) / FCPPT_TEXT("gravity") / FCPPT_TEXT("default-gravity"))),
	target_gravity_(
		fruitlib::json::find_and_convert_member<fruitlib::physics::vector3>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("bonsu")) / FCPPT_TEXT("gravity") / FCPPT_TEXT("decreased-gravity"))),
	timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitlib::time_format::find_and_convert_duration<ingame_timer::duration>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("bonsu")) / FCPPT_TEXT("gravity") / FCPPT_TEXT("duration")))
			.expired(
				true))
{
}

void
fruitapp::bonsu::instance::gravity::activate()
{
	timer_.reset();
}

fruitapp::bonsu::instance::gravity::~gravity()
{
}

void
fruitapp::bonsu::instance::gravity::update()
{
	world_.gravity(
		this->dead()
		?
			default_gravity_
		:
			target_gravity_);
}

fruitapp::bonsu::texture_identifier const
fruitapp::bonsu::instance::gravity::texture() const
{
	return 
		FCPPT_TEXT("gravity");
}

bool
fruitapp::bonsu::instance::gravity::dead() const
{
	return 
		timer_.expired();
}

fruitapp::bonsu::progress_fraction
fruitapp::bonsu::instance::gravity::progress() const
{
	return 
		sge::timer::elapsed_fractional<bonsu::progress_fraction>(
			timer_);
}

