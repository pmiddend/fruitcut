#ifndef FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_SIMPLE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_SIMPLE_HPP_INCLUDED

#include "base.hpp"
#include "../../animation.hpp"
#include <sge/renderer/vector2.hpp>
#include <sge/sprite/object.hpp>
#include <sge/sprite/parameters_fwd.hpp>
#include <sge/time/timer.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace particle
{
namespace objects
{
template<typename Choices>
class simple
:
	public base
{
public:
	typedef
	Choices
	choices;

	typedef
	sge::sprite::object<choices>
	sprite_object;

	typedef
	sge::sprite::parameters<choices>
	sprite_parameters;

	typedef typename
	sprite_object::color_type
	color_type;

	typedef
	animation<color_type>
	color_animation;

	explicit
	simple(
		sprite_parameters const &,
		typename color_animation::value_sequence const &,
		sge::time::callback const &,
		sge::renderer::vector2 const &velocity,
		sge::renderer::vector2 const &acceleration);

	void
	update();

	bool
	dead() const;

	void
	velocity(
		sge::renderer::vector2 const &);

	void
	acceleration(
		sge::renderer::vector2 const &);

	~simple();
private:
	sprite_object sprite_;
	color_animation animation_;
	sge::time::timer seconds_timer_;
	sge::renderer::vector2 position_;
	sge::renderer::vector2 velocity_;
	sge::renderer::vector2 acceleration_;
};
}
}
}
}

#endif
