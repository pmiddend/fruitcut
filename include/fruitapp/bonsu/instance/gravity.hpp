#ifndef FRUITAPP_BONSU_INSTANCE_GRAVITY_HPP_INCLUDED
#define FRUITAPP_BONSU_INSTANCE_GRAVITY_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/bonsu/manager_fwd.hpp>
#include <fruitapp/bonsu/instance/base.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/vector/basic_impl.hpp>


namespace fruitapp
{
namespace bonsu
{
namespace instance
{
class gravity
:
	public instance::base
{
FCPPT_NONCOPYABLE(
	gravity);
public:
	explicit
	gravity(
		bonsu::manager &,
		sge::parse::json::object const &,
		fruitlib::physics::world &,
		fruitapp::ingame_clock const &);

	void
	activate();

	~gravity();
private:
	fruitlib::physics::world &world_;
	fruitlib::physics::vector3 const default_gravity_;
	fruitlib::physics::vector3 const target_gravity_;
	fruitapp::ingame_timer timer_;

	void
	update();

	bonsu::texture_identifier const
	texture() const;

	bool
	dead() const;

	bonsu::progress_fraction
	progress() const;
};
}
}
}

#endif
