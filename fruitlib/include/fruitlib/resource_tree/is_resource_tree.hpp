#ifndef FRUITLIB_RESOURCE_TREE_IS_RESOURCE_TREE_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_IS_RESOURCE_TREE_HPP_INCLUDED

#include <fruitlib/resource_tree/node.hpp>
#include <fcppt/container/tree/object.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/bool.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace resource_tree
{
FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

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

FCPPT_PP_POP_WARNING
}
}

#endif
