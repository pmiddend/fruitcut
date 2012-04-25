#ifndef FRUITLIB_UNIFORM_RANDOM_RANGE_ELEMENT_DECL_HPP_INCLUDED
#define FRUITLIB_UNIFORM_RANDOM_RANGE_ELEMENT_DECL_HPP_INCLUDED

#include <fruitlib/uniform_random_range_element_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/random/variate.hpp>
#include <fcppt/random/distribution/uniform_int.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/range/difference_type.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/value_type.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
template<typename Range,typename RandomGenerator>
class uniform_random_range_element
{
FCPPT_NONCOPYABLE(
	uniform_random_range_element);
public:
	typedef
	Range
	range_type;

	typedef
	RandomGenerator
	rng_type;

	typedef typename
	boost::range_reference<range_type>::type
	reference_type;

	typedef typename
	boost::range_size<range_type>::type
	size_type;

	typedef typename
	boost::range_difference<range_type>::type
	difference_type;

	typedef typename
	boost::range_iterator<range_type>::type
	iterator_type;

	uniform_random_range_element(
		rng_type &,
		range_type &);

	reference_type
	value();

	iterator_type
	iterator();

	~uniform_random_range_element();
private:
	typedef
	fcppt::random::distribution::uniform_int<difference_type>
	uniform_distribution_type;

	typedef
	fcppt::random::variate<rng_type,uniform_distribution_type>
	variate_type;

	range_type &range_;
	size_type const size_;
	variate_type variate_;
};
}

#endif

