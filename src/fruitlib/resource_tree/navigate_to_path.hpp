#ifndef FRUITLIB_RESOURCE_TREE_NAVIGATE_TO_PATH_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_NAVIGATE_TO_PATH_HPP_INCLUDED

#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/exception.hpp>
#include <fruitlib/resource_tree/is_resource_tree.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>

namespace fruitlib
{
namespace resource_tree
{
namespace detail
{
template<typename T>
struct iterator_chooser
{
	typedef typename
	boost::mpl::if_
	<
		boost::is_const<T>,
		typename T::const_iterator,
		typename T::iterator
	>::type
	type;
};

template<typename Tree>
typename
boost::enable_if_c
<
	resource_tree::is_resource_tree<Tree>::value,
	typename boost::mpl::if_
	<
		boost::is_const<Tree>,
		Tree const &,
		Tree &
	>::type
>::type
navigate_to_path(
	Tree &t,
	resource_tree::path const &p,
	resource_tree::path::value_sequence::const_iterator current_it)
{
	typedef
	Tree
	tree_type;

	typedef typename
	iterator_chooser<tree_type>::type
	iterator;

	for(iterator i = t.begin(); i != t.end(); ++i)
	{
		if(i->value().name() != *current_it)
			continue;

		if(current_it == boost::prior(p.values().end()))
			return *i;

		return 
			detail::navigate_to_path(
				t,
				p,
				++current_it);
	}

	throw fruitlib::exception(FCPPT_TEXT("Couldn't locate path ")+p.string()+FCPPT_TEXT(", stopped at ")+(*current_it));
}
}

template<typename Tree>
typename
boost::enable_if_c
<
	resource_tree::is_resource_tree<Tree>::value,
	typename boost::mpl::if_
	<
		boost::is_const<Tree>,
		Tree const &,
		Tree &
	>::type
>::type
navigate_to_path(
	Tree &t,
	resource_tree::path const &p)
{
	return 
		detail::navigate_to_path(
			t,
			p,
			p.values().begin());
}
}
}

#endif
