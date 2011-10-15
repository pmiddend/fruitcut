#ifndef FRUITLIB_RESOURCE_TREE_PATH_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_PATH_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <vector>

namespace fruitlib
{
namespace resource_tree
{
class path
{
public:
	typedef
	std::vector<fcppt::string>
	value_sequence;

	explicit
	path(
		fcppt::string const &);

	path &
	operator/(
		fcppt::string const &);

	value_sequence const &
	values() const;

	fcppt::string const
	string() const;
private:
	value_sequence values_;
};
}
}

#endif
