#ifndef FRUITLIB_RESOURCE_TREE_OBJECT_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_OBJECT_HPP_INCLUDED

#include <fcppt/container/tree/object.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/string.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <utility>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace resource_tree
{
template<typename LeafType,typename NodeType>
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	typedef
	LeafType
	leaf_type;

	typedef
	NodeType
	node_type;

	explicit
	object()
	:
		container_()
	{
	}

	~object()
	{
	}
};
}
}

#endif
