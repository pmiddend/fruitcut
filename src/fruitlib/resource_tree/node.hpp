#ifndef FRUITLIB_RESOURCE_TREE_NODE_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_NODE_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace resource_tree
{
namespace detail
{
template<typename Leaf,typename Node>
struct is_leaf_visitor
{
public:
	typedef
	bool
	result_type;

	// Yeah, could be just return is_same etc.
	result_type
	operator()(
		Leaf const &) const
	{
		return true;
	}

	result_type
	operator()(
		Node const &) const
	{
		return false;
	}
};
}

template<typename Leaf,typename Node>
class node
{
public:
	typedef
	Leaf
	leaf_type;

	typedef
	Node
	node_type;

	typedef
	fcppt::variant::object
	<
		boost::mpl::vector2<leaf_type, node_type>
	>
	value_type;

	explicit
	node(
		fcppt::string const &_name,
		value_type const &_value)
	:
		name_(
			_name),
		value_(
			_value)
	{
	}

	fcppt::string const &
	name() const
	{
		return name_;
	}

	value_type const &
	value() const
	{
		return value_;
	}

	value_type &
	value()
	{
		return value_;
	}

	void
	value(
		value_type const &_value)
	{
		value_ = _value;
	}

	bool
	is_leaf() const
	{
		return
			fcppt::variant::apply_unary(
				detail::is_leaf_visitor<leaf_type,node_type>(),
				value_);
	}

	leaf_type const &
	leaf_value() const
	{
		return
			value_.template get<leaf_type>();
	}

	leaf_type &
	leaf_value()
	{
		return
			value_.template get<leaf_type>();
	}

	node_type const &
	node_value() const
	{
		return
			value_.template get<node_type>();
	}

	node_type &
	node_value()
	{
		return
			value_.template get<node_type>();
	}
private:
	fcppt::string name_;
	value_type value_;
};
}
}

#endif
