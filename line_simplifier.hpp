#ifndef FRUITCUT_LINE_SIMPLIFIER_HPP_INCLUDED
#define FRUITCUT_LINE_SIMPLIFIER_HPP_INCLUDED

#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/angle_between.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

namespace fruitcut
{
// The requirement for this class is that Container contains instances
// of some fcppt::math::vector.
//
// It takes a container as template argument (rather than a vector)
// because we want to use it with a circular_buffer as "backend"
// container.
template<typename Container>
class line_simplifier
{
public:
	typedef 
	Container
	container;

	typedef typename
	Container::value_type
	point;

	typedef typename
	Container::iterator
	iterator;

	typedef typename
	Container::const_iterator
	const_iterator;

	typedef typename
	point::value_type
	scalar;

	explicit
	line_simplifier(
		Container const &_points,
		scalar const _epsilon,
		scalar const _angle_threshold)
	:
		points_(
			_points),
		epsilon_(
			_epsilon),
		angle_threshold_(
			_angle_threshold)
	{
	}

	// It would be great if we could simplify the container as points
	// get added to it but unfortunately, simplification with the
	// current method is not associative.
	template<typename Result>
	Result const
	simplify() const
	{
		Result result;

		if (points_.empty())
			return result;

		const_iterator 
			current = 
				boost::next(
					points_.begin());

		while (current != points_.end())
		{
			point const diff = 
				*current - *boost::prior(current);

			const_iterator new_current = 
				boost::next(
					current);

			while(
				new_current != points_.end() && 
				fcppt::math::vector::angle_between(
					diff,
					*new_current - *boost::prior(current)) < angle_threshold_)
				++new_current;

			result.push_back(
				*boost::prior(current));

			result.push_back(
				*boost::prior(new_current));

			current = 
				new_current;
		}
		
		return result;
	}

	void
	push_back(
		point const &p)
	{
		if(
			fcppt::math::vector::length(
				p - points_.back()) > epsilon_)
			points_.push_back(
				p);
	}
private:
	container points_;
	scalar epsilon_,angle_threshold_;
};
}

#endif
