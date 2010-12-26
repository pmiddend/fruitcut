#ifndef FRUITCUT_PARTICLE_OBJECTS_SIMPLE_HPP_INCLUDED
#define FRUITCUT_PARTICLE_OBJECTS_SIMPLE_HPP_INCLUDED

#include "base.hpp"
#include "../../sprite/parameters.hpp"
#include "../../sprite/object.hpp"
#include "../../animation.hpp"
#include <sge/renderer/vector2.hpp>
#include <sge/time/timer.hpp>

namespace fruitcut
{
namespace particle
{
namespace objects
{
class simple
:
	public base
{
public:
	explicit
	simple(
		sprite::parameters const &,
		animation::value_sequence const &,
		sge::renderer::vector2 const &velocity,
		sge::renderer::vector2 const &acceleration);

	void
	update();

	bool
	dead() const;

	void
	acceleration(
		sge::renderer::vector2 const &);

	~simple();
private:
	sprite::object sprite_;
	animation animation_;
	sge::time::timer seconds_timer_;
	sge::renderer::vector2 position_;
	sge::renderer::vector2 velocity_;
	sge::renderer::vector2 acceleration_;
};
}
}
}

#endif
