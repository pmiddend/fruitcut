#ifndef FRUITCUT_MATH_GRAHAM_SCAN_HPP_INCLUDED
#define FRUITCUT_MATH_GRAHAM_SCAN_HPP_INCLUDED

// NOTE: This isn't used anywhere, it's just an attempt to simplify
// cut_mesh.cpp
#include "nearly_equals.hpp"
#include "compare_angles.hpp"
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/operator/if_else.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include <algorithm>
#include <set>
#include <cmath>

namespace fruitcut
{
namespace math
{
IndexSequence
graham_scan(
	IndexSequence indices,
	GetVector const &get_vector)
{
	typedef
	IndexSequence
	index_sequence;

	typedef typename
	GetVector::result_type::value_type
	scalar;

	typedef typename 
	index_sequence::iterator
	iterator;

	typedef typename
	GetVector::result_type
	vector;

	scalar const epsilon = 
		static_cast<scalar>(
			0.00001);

	iterator min_y_it = 
		boost::min_element(
			indices.begin(),
			boost::phoenix::if_else(
				boost::phoenix::bind(
					&nearly_equals<scalar>,
					boost::phoenix::bind(
						get_vector,
						boost::phoenix::arg_names::arg1)[1],
					boost::phoenix::bind(
						get_vector,
						boost::phoenix::arg_names::arg2)[1],
					epsilon),
				boost::phoenix::bind(
					get_vector,
					boost::phoenix::arg_names::arg1)[0] < 
				boost::phoenix::bind(
					get_vector,
					boost::phoenix::arg_names::arg2)[0],
				boost::phoenix::bind(
					get_vector,
					boost::phoenix::arg_names::arg1)[1] < 
				boost::phoenix::bind(
					get_vector,
					boost::phoenix::arg_names::arg2)[1]));

	std::swap(
		*min_y_it,
		*indices.begin());

	vector const min_y = 
		*input.begin();

	typedef
	std::set<iterator>
	index_set;

	// I think this is technically an optimization: If "min_y" and two
	// other points are colinear, delete the nearest point after min_y.
	// TODO: Can we make this more pretty?
	for(
		pair_iterator<iterator> pit(
			boost::next(
				indices.begin()),
			indices.end());
		pit.at_end();
		++pit)
	{
		vector const 
			first = 
				get_vector(
					*pit->first),
			second = 
				get_vector(
					*pit->second);

		if(
			std::abs(
				compare_angles<scalar>(
					min_y,
					first,
					second)) < epsilon)
			to_delete.insert(
				fcppt::math::vector::length(
					min_y - first) < 
				fcppt::math::vector::length(
					min_y - second)
				?
					pit->first
				:
					pit->second);
	}

	BOOST_FOREACH(
		typename index_set::const_reference r,
		to_delete)
		indices.erase(
			boost::next(
				indices.begin(),
				r));

	std::sort(
		boost::next(
			indices.begin()),
		indices.end(),
		boost::phoenix::bind(
			&compare_angles<scalar>,
			min_y,
			boost::phoenix::bind(
				get_vector,
				boost::phoenix::arg_names::arg1),
			boost::phoenix::bind(
				get_vector,
				boost::phoenix::arg_names::arg2)) 
			< static_cast<scalar>(0));

	return indices;
}
}
}

#endif
