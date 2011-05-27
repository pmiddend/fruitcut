#ifndef FRUITCUT_RESOURCE_TREE_NODE_NAME_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_NODE_NAME_HPP_INCLUDED

#include "make_type.hpp"
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/string.hpp>
#include <utility>

namespace fruitcut
{
namespace resource_tree
{
namespace detail
{
template<typename T>
class node_name_visitor
{
public:
	typedef
	fcppt::string
	result_type;

	result_type const
	operator()(
		std::pair<fcppt::string,T> const &p) const
	{
		return p.first;
	}

	result_type const
	operator()(
		fcppt::string const &p) const
	{
		return p;
	}
};
}

template<typename T>
fcppt::string const
node_name(
	typename resource_tree::make_type<T>::type const &node)
{
	return 
		fcppt::variant::apply_unary(
			resource_tree::detail::node_name_visitor<T>(),
			node.value());
}
}
}

#endif
