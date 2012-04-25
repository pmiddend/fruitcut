#ifndef FRUITLIB_UNIFORM_RANDOM_RANGE_ELEMENT_IMPL_HPP_INCLUDED
#define FRUITLIB_UNIFORM_RANDOM_RANGE_ELEMENT_IMPL_HPP_INCLUDED

#include <fruitlib/uniform_random_range_element_decl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <fcppt/config/external_end.hpp>


template<typename Range,typename RandomGenerator>
fruitlib::uniform_random_range_element<Range,RandomGenerator>::uniform_random_range_element(
	rng_type &_random_generator,
	range_type &_range)
:
	range_(
		_range),
	size_(
		// Broken boost::size
		static_cast<size_type>(
			boost::size(
				_range))),
	variate_(
		_random_generator,
		uniform_distribution_type(
			typename uniform_distribution_type::min(
				0),
			typename uniform_distribution_type::max(
				size_
				?
					static_cast<difference_type>(
						size_ -
						1u)
				:
					static_cast<difference_type>(
						0))))
{
}

template<typename Range,typename RandomGenerator>
typename
fruitlib::uniform_random_range_element<Range,RandomGenerator>::reference_type
fruitlib::uniform_random_range_element<Range,RandomGenerator>::value()
{
	return
		*(this->iterator());
}

template<typename Range,typename RandomGenerator>
typename
fruitlib::uniform_random_range_element<Range,RandomGenerator>::iterator_type
fruitlib::uniform_random_range_element<Range,RandomGenerator>::iterator()
{
	return
		boost::next(
			boost::begin(
				range_),
			variate_());
}

template<typename Range,typename RandomGenerator>
fruitlib::uniform_random_range_element<Range,RandomGenerator>::~uniform_random_range_element()
{
}

#endif

