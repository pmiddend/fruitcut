#ifndef FRUITCUT_RESOURCE_TREE_NAVIGATE_TO_PATH_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_NAVIGATE_TO_PATH_HPP_INCLUDED

#include "make_type.hpp"
#include "path.hpp"
#include "node_name.hpp"
#include "../exception.hpp"
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>

namespace fruitcut
{
namespace resource_tree
{
namespace detail
{
template<typename T>
typename 
resource_tree::make_type<T>::type const &
navigate_to_path(
	typename resource_tree::make_type<T>::type const &t,
	resource_tree::path const &p,
	resource_tree::path::value_sequence::const_iterator current_it)
{
	typedef typename
	resource_tree::make_type<T>::type
	tree_type;

	typedef typename
	tree_type::const_iterator
	iterator;

	for(iterator i = t.begin(); i != t.end(); ++i)
	{
		if(resource_tree::node_name<T>(*i) != *current_it)
			continue;

		if(current_it == boost::prior(p.values().end()))
			return *i;

		return 
			detail::navigate_to_path<T>(
				t,
				p,
				++current_it);
	}

	throw fruitcut::exception(FCPPT_TEXT("Couldn't locate path ")+p.string()+FCPPT_TEXT(", stopped at ")+(*current_it));
}
}

template<typename T>
typename 
resource_tree::make_type<T>::type const &
navigate_to_path(
	typename resource_tree::make_type<T>::type const &t,
	resource_tree::path const &p)
{
	return 
		detail::navigate_to_path<T>(
			t,
			p,
			p.values().begin());
}
}
}

#endif
