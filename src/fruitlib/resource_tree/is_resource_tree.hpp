#ifndef FRUITLIB_RESOURCE_TREE_IS_RESOURCE_TREE_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_IS_RESOURCE_TREE_HPP_INCLUDED

#include "node.hpp"
#include <fcppt/container/tree/object.hpp>
#include <boost/mpl/bool.hpp>

namespace fruitlib
{
namespace resource_tree
{
template<typename T>
struct is_resource_tree : boost::mpl::false_ {};

template<typename T,typename U>
struct is_resource_tree
<
	fcppt::container::tree::object
	<
		resource_tree::node<T,U>
	>
> : boost::mpl::true_ {};
}
}

#endif
