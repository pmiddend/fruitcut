#ifndef FRUITCUT_JSON_ARRAY_TO_VECTOR_HPP_INCLUDED
#define FRUITCUT_JSON_ARRAY_TO_VECTOR_HPP_INCLUDED

#include "convert.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/element_vector.hpp>
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
	for(
		sge::parse::json::element_vector::const_iterator r = 
			a.elements.begin();
		r != a.elements.end();
		++r)
		result.push_back(
			convert<T>(
				*r));
	return result;
}
}
}

#endif
