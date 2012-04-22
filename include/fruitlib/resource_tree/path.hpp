#ifndef FRUITLIB_RESOURCE_TREE_PATH_HPP_INCLUDED
#define FRUITLIB_RESOURCE_TREE_PATH_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


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

	path const
	operator/(
		fcppt::string const &) const;

	value_sequence const &
	values() const
	FCPPT_PP_CONST;

	fcppt::string const
	string() const
	FCPPT_PP_PURE;
private:
	value_sequence values_;
};
}
}

#endif
