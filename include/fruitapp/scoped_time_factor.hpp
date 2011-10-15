#ifndef FRUITAPP_SCOPED_TIME_FACTOR_HPP_INCLUDED
#define FRUITAPP_SCOPED_TIME_FACTOR_HPP_INCLUDED

#include <fruitapp/machine_fwd.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
class scoped_time_factor
{
FCPPT_NONCOPYABLE(
	scoped_time_factor);
public:
	explicit
	scoped_time_factor(
		machine &,
		fruitapp::ingame_clock::float_type);

	~scoped_time_factor();
private:
	machine &m_;
	fruitapp::ingame_clock::float_type const old_factor_;
};
}

#endif
