#ifndef FRUITCUT_ANIMATION_HPP_INCLUDED
#define FRUITCUT_ANIMATION_HPP_INCLUDED

#include "sprite/object.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/duration.hpp>
#include <sge/sprite/object_impl.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/math/lerp.hpp>
#include <fcppt/assert.hpp>
#include <boost/next_prior.hpp>
#include <mizuiro/color/operators/add.hpp>
#include <mizuiro/color/operators/scalar_multiply.hpp>
#include <vector>
#include <utility>

namespace fruitcut
{
template<typename T>
class animation
{
FCPPT_NONCOPYABLE(
	animation)
public:
	typedef
	std::pair
	<
		sge::time::duration,
		T
	>
	value_type;

	typedef
	std::vector<value_type>
	value_sequence;

	explicit
	animation(
		value_sequence const &_values)
	:
		values_(
			_values),
		current_timer_(
			new sge::time::timer(
				values_.front().first)),
		current_value_(
			values_.begin())
	{
		FCPPT_ASSERT(
			!values_.empty());
	}

	void
	update()
	{
		if (current_value_ == values_.end())
			return;

		if (!current_timer_->expired())
			return;

		++current_value_;
		if (current_value_ == boost::prior(values_.end()))
			return;
		current_timer_.reset(
			new sge::time::timer(
				current_value_->first));
	}

	sprite::object::color_type const
	current_value() const
	{
		if (current_value_ == boost::prior(values_.end()))
			return current_value_->second;

		sge::time::funit const v = 
			// The timer could have expired but update didn't notice it, yet
			current_timer_->expired()
			?
				static_cast<sge::time::funit>(1)
			:
				current_timer_->elapsed_frames();

		return 
			fcppt::math::lerp(
				v,
				boost::next(
					current_value_)->second,
				current_value_->second);
	}

	bool 
	finished() const
	{
		return 
			current_value_ == 
				boost::prior(
					values_.end()) && 
			current_timer_->expired();
	}
private:
	value_sequence const values_;
	fcppt::scoped_ptr<sge::time::timer> current_timer_;
	typename value_sequence::const_iterator current_value_;
};
}

#endif
