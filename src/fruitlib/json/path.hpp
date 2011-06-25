#ifndef FRUITCUT_FRUITLIB_JSON_PATH_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_PATH_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <vector>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
class path
{
public:
	typedef
	std::vector<fcppt::string>
	sequence_type;

	typedef
	sequence_type::const_iterator
	const_iterator;

	typedef
	sequence_type::const_reference
	const_reference;
	
	typedef
	sequence_type::size_type
	size_type;

	explicit
	path(
		const_iterator,
		const_iterator);

	explicit
	path(
		fcppt::string const &);

	const_iterator 
	begin() const;

	const_iterator 
	end() const;

	size_type
	size() const;

	const_reference
	back() const;

	bool
	empty() const;

	path &
	operator/(
		fcppt::string const &);
private:
	sequence_type sequence_;
};
}
}
}

#endif
