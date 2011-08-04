#ifndef FRUITLIB_ANIMATION_KEYFRAME_HPP_INCLUDED
#define FRUITLIB_ANIMATION_KEYFRAME_HPP_INCLUDED

#include <fcppt/chrono/duration_impl.hpp>

namespace fruitlib
{
namespace animation
{
template<typename Value,typename Clock>
class keyframe
{
public:
	typedef 
	Value
	value_type;
	
	typedef typename
	Clock::duration
	duration_type;

	explicit
	keyframe(
		value_type const &_value,
		duration_type const &_duration)
	:
		value_(
			_value),
		duration_(
			_duration)
	{
	}

	value_type const &
	value() const
	{
		return value_;
	}

	duration_type const &
	duration() const
	{
		return duration_;
	}
private:
	value_type value_;
	duration_type duration_;
};
}
}

#endif
