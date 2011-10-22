#ifndef FRUITLIB_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED

#include <fruitlib/resource_tree/make_type.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/is_directory.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/function/object.hpp>


namespace fruitlib
{
namespace resource_tree
{
template<typename Tree>
fcppt::unique_ptr<Tree>
from_directory_tree(
	fcppt::filesystem::path const &root,
	fcppt::function::object
	<
		typename Tree::value_type::leaf_type (
			fcppt::filesystem::path const &)
	> const &create_leaf,
	fcppt::function::object
	<
		typename Tree::value_type::node_type (
			fcppt::filesystem::path const &)
	> const &create_node)
{
	typedef
	Tree
	tree_type;

	typedef typename
	tree_type::value_type
	node_type;

	if(!fcppt::filesystem::is_directory(root))
	{
		return
			fcppt::make_unique_ptr<tree_type>(
				node_type(
					fcppt::filesystem::stem(
						root),
					create_leaf(
						root)));
	}

	fcppt::unique_ptr<tree_type> result(
		fcppt::make_unique_ptr<tree_type>(
			node_type(
				fcppt::filesystem::stem(
					root),
				create_node(
					root))));

	for(
		fcppt::filesystem::directory_iterator current_file(
			root);
		current_file != fcppt::filesystem::directory_iterator();
		++current_file)
	{
		result->push_back(
			resource_tree::from_directory_tree<Tree>(
				*current_file,
				create_leaf,
				create_node));
	}

	return
		fcppt::move(
			result);
}
}
}

#endif
