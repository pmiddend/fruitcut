#ifndef FRUITCUT_FRUITLIB_RESOURCE_TREE_MAKE_TYPE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_RESOURCE_TREE_MAKE_TYPE_HPP_INCLUDED

#include "node.hpp"
#include <fcppt/container/tree/object.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace resource_tree
{
template<typename LeafType,typename NodeType>
struct make_type
{
	typedef
	fcppt::container::tree::object
	<
		node<LeafType,NodeType>
	>
	type;
};
}
}
}

#endif
