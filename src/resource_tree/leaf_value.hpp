#ifndef FRUITCUT_RESOURCE_TREE_LEAF_VALUE_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_LEAF_VALUE_HPP_INCLUDED

#include "make_type.hpp"
#include <fcppt/string.hpp>
#include <utility>

namespace fruitcut
{
namespace resource_tree
{
template<typename T>
T const &
leaf_value(
	typename resource_tree::make_type<T>::type const &input_tree)
{
	return 
		input_tree.value().template get < std::pair<fcppt::string,T> >().second;
}
}
}

#endif
