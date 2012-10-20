#ifndef FRUITLIB_RESOURCE_TREE_OBJECT_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_OBJECT_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/container/tree/object.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


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

	object()
	{
	}

	~object()
	{
	}
};
}
}

#endif
