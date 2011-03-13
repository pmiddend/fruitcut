#ifndef FRUITCUT_JSON_ARRAY_TO_VECTOR_HPP_INCLUDED
#define FRUITCUT_JSON_ARRAY_TO_VECTOR_HPP_INCLUDED

#include "convert.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <boost/foreach.hpp>
#include <vector>

namespace fruitcut
{
namespace json
{
template<typename T>
std::vector<T> const
array_to_vector(
	sge::parse::json::array const &a)
{
	std::vector<T> result;
	BOOST_FOREACH(
		sge::parse::json::element_vector::const_reference r,
		a.elements)
		result.push_back(
			convert<T>(
				r));
	return result;
}
}
}

#endif
