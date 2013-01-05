#ifndef FRUITLIB_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED

#include <fruitlib/resource_tree/make_type.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <functional>
#include <memory>
#include <utility>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace resource_tree
{
template<typename Tree>
std::unique_ptr<Tree>
from_directory_tree(
	boost::filesystem::path const &root,
	std::function
	<
		typename Tree::value_type::leaf_type (
			boost::filesystem::path const &)
	> const &create_leaf,
	std::function
	<
		typename Tree::value_type::node_type (
			boost::filesystem::path const &)
	> const &create_node)
{
	typedef
	Tree
	tree_type;

	typedef typename
	tree_type::value_type
	node_type;

	if(!boost::filesystem::is_directory(root))
	{
		return
			fcppt::make_unique_ptr<tree_type>(
				node_type(
					fcppt::filesystem::stem(
						root),
					create_leaf(
						root)));
	}

	std::unique_ptr<tree_type> result(
		fcppt::make_unique_ptr<tree_type>(
			node_type(
				fcppt::filesystem::stem(
					root),
				create_node(
					root))));

	for(
		boost::filesystem::directory_iterator current_file(
			root);
		current_file != boost::filesystem::directory_iterator();
		++current_file)
	{
		result->push_back(
			resource_tree::from_directory_tree<Tree>(
				*current_file,
				create_leaf,
				create_node));
	}

	return
		std::move(
			result);
}
}
}

#endif
