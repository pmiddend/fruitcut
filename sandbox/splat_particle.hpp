#ifndef FRUITCUT_SANDBOX_SPLAT_PARTICLE_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SPLAT_PARTICLE_HPP_INCLUDED

#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/renderer/vector2.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitcut
{
namespace sandbox
{
class splat_particle
{
public:
	typedef
	fcppt::chrono::high_resolution_clock
	clock;

	typedef
	clock::time_point
	time_point;

	typedef
	clock::duration
	duration;

	explicit
	splat_particle(
		sprite::parameters const &,
		sge::time::duration const &life_time,
		sge::renderer::vector2 const &velocity,
		sge::renderer::vector2 const &acceleration);

	void
	update();

	bool
	dead() const;

	~splat_particle();
private:
	sprite::object sprite_;
	sge::time::timer timer_;
	sge::time::timer second_timer_;
	sge::renderer::vector2 position_;
	sge::renderer::vector2 velocity_;
	sge::renderer::vector2 acceleration_;
};
}
}

#endif
