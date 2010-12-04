#ifndef FRUITCUT_SANDBOX_PARTICLE_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PARTICLE_HPP_INCLUDED

#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/object_impl.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace sandbox
{
class particle
{
public:
	typedef
	fcppt::chrono::high_resolution_clock::time_point
	time_point;

	typedef
	fcppt::chrono::high_resolution_clock::duration
	duration;

	explicit
	particle(
		sprite::parameters const &,
		duration const &life_time);

	void
	update();

	bool
	dead() const;

	~particle();
private:
	sprite::object sprite_;
	sprite::object::dim const initial_size_;
	time_point birth_;
	duration life_time_;
};
}
}

#endif
