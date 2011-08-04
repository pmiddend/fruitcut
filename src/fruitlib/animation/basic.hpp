#ifndef FRUITLIB_ANIMATION_BASIC_HPP_INCLUDED
#define FRUITLIB_ANIMATION_BASIC_HPP_INCLUDED

#include <fruitlib/animation/keyframe.hpp>
#include <fruitlib/exception.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>
#include <vector>

namespace fruitlib
{
namespace animation
{
template
<
	typename Value,
	typename Clock,
	typename Interpolator,
	typename Float = float
>
class basic
{
FCPPT_NONCOPYABLE(
	basic);
public:
	typedef
	Value
	value_type;

	typedef
	Clock
	clock;

	typedef typename
	Clock::duration
	duration;

	typedef
	Float
	float_type;

	typedef
	animation::keyframe<value_type,clock>
	keyframe;

	typedef
	std::vector<keyframe>
	keyframe_sequence;

	typedef typename
	keyframe_sequence::const_iterator
	keyframe_iterator;

	typedef
	sge::timer::basic<clock>
	timer;

	typedef typename
	sge::timer::basic<clock>::parameters
	timer_parameters;

	explicit
	basic(
		keyframe_sequence const &_keyframes,
		clock const &_clock)
	:
		keyframes_(
			_keyframes),
		current_keyframe_(
			keyframes_.begin()),
		current_timer_(
			timer_parameters(
				_clock,
				keyframes_.empty()
				?
					throw 
						fruitlib::exception(
							FCPPT_TEXT("You didn't specify any keyframes for the animation"))
				:
					keyframes_.front().duration()))
	{
	}

	void
	update()
	{
		if(current_keyframe_ == keyframes_.end() || !current_timer_.expired())
			return;

		++current_keyframe_;		

		// This is safe because keyframes_ cannot be empty (see ctor)
		if(current_keyframe_ == --keyframes_.end())
			return;

		current_timer_.reset();
		current_timer_.interval(
			current_keyframe_.duration());
	}

	value_type const
	current_value() const
	{
		// This is safe because keyframes_ cannot be empty (see ctor)
		if(current_keyframe_ == --keyframes_.end())
			return 
				current_keyframe_->value();

		float_type const v = 
			// The timer could have expired but update didn't notice it, yet
			current_timer_->expired()
			?
				static_cast<float_type>(1)
			:
				sge::timer::elapsed_fractional<float_type>(
					current_timer_);

		return 
			Interpolator()(
				v,
				current_keyframe_->value(),
				boost::next(
					current_keyframe_)->value());
	}

	bool 
	finished() const
	{
		return 
			current_keyframe_ == --keyframes_.end() &&
			current_timer_->expired();
	}

	~basic()
	{
	}
private:
	keyframe_sequence keyframes_;
	keyframe_iterator current_keyframe_;
	timer current_timer_;
};
}
}

#endif
