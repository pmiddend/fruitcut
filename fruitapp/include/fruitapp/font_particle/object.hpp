#ifndef FRUITAPP_FONT_PARTICLE_OBJECT_HPP_INCLUDED
#define FRUITAPP_FONT_PARTICLE_OBJECT_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/font_particle/lifetime.hpp>
#include <fruitapp/font_particle/position.hpp>
#include <fruitapp/font_particle/text.hpp>
#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/image/color/any/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace font_particle
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object(
		fruitlib::scenic::optional_parent const &,
		fruitlib::font::manager &,
		fruitlib::font::identifier const &,
		fruitapp::font_particle::text const &,
		fruitapp::font_particle::position const &,
		fruitapp::ingame_clock const &,
		fruitapp::font_particle::lifetime const &,
		sge::image::color::any::object const &);

	bool
	dead() const;

	void
	update();

	~object();
private:
	fruitlib::font::scene_node node_;
	fruitapp::ingame_timer lifetimer_;
};
}
}

#endif
