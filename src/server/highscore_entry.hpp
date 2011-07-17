#ifndef FRUITCUT_SERVER_HIGHSCORE_ENTRY_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_ENTRY_HPP_INCLUDED

#include "ascii/string.hpp"
#include "score.hpp"

namespace fruitcut
{
namespace server
{
class highscore_entry
{
public:
	explicit
	highscore_entry();

	explicit
	highscore_entry(
		ascii::string const &name,
		server::score,
		ascii::string const &datetime);

	ascii::string const &
	name() const;

	server::score
	score() const;

	ascii::string const &
	datetime() const;

	bool
	operator==(
		highscore_entry const &) const;
private:
	ascii::string name_;
	server::score score_;
	ascii::string datetime_;
};
}
}

#endif
