#ifndef FRUITCUT_SANDBOX_SWORD_PARTICLE_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SWORD_PARTICLE_HPP_INCLUDED

#include "../../particle/sprite/parameters.hpp"
#include "../../particle/sprite/object.hpp"
#include <sge/sprite/object_impl.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace sandbox
{
class sword_particle
{
public:
	typedef
	fcppt::chrono::high_resolution_clock::time_point
	time_point;

	typedef
	fcppt::chrono::high_resolution_clock::duration
	duration;

	explicit
	sword_particle(
		particle::sprite::parameters const &,
		duration const &life_time);

	void
	update();

	bool
	dead() const;

	~sword_particle();
private:
	particle::sprite::object sprite_;
	particle::sprite::object::dim const initial_size_;
	time_point birth_;
	duration life_time_;
};
}
}

#endif
