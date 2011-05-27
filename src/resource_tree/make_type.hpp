#ifndef FRUITCUT_RESOURCE_TREE_MAKE_TYPE_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_MAKE_TYPE_HPP_INCLUDED

#include <fcppt/container/tree/object.hpp>
#include <fcppt/variant/variant.hpp>
#include <fcppt/string.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <utility>

namespace fruitcut
{
namespace resource_tree
{
template<typename T>
struct make_type
{
	typedef
	fcppt::container::tree::object
	<
		fcppt::variant::object
		<
			boost::mpl::vector2
			<
				fcppt::string,
				std::pair<fcppt::string,T>
			>
		>
	>
	type;
};
}
}

#endif
