#ifndef FRUITCUT_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_FROM_DIRECTORY_TREE_HPP_INCLUDED

#include "make_type.hpp"
#include <fcppt/filesystem/path.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/filesystem/is_directory.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <utility>

namespace fruitcut
{
namespace resource_tree
{
template<typename T>
fcppt::unique_ptr
<
	typename resource_tree::make_type<T>::type
>
from_directory_tree(
	fcppt::filesystem::path const &root,
	fcppt::function::object<T(fcppt::filesystem::path const &)> const &create_resource)
{
	typedef typename
	resource_tree::make_type<T>::type 
	tree_type;

	fcppt::unique_ptr<tree_type> result(
		fcppt::make_unique_ptr<tree_type>());

	FCPPT_ASSERT(
		fcppt::filesystem::exists(
			root));

	if(!fcppt::filesystem::is_directory(root))
	{
		result->value(
			std::make_pair(
				fcppt::filesystem::stem(
					root),
				create_resource(
					root)));

		return 
			fcppt::move(
				result);
	}

	result->value(
		fcppt::filesystem::stem(
			root));

	for(
		fcppt::filesystem::directory_iterator current_file(
			root);
		current_file != fcppt::filesystem::directory_iterator();
		++current_file)
	{
		result->push_back(
			resource_tree::from_directory_tree(
				*current_file,
				create_resource));
	}

	return 
		fcppt::move(
			result);
}
}
}

#endif
