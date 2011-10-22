#ifndef FRUITSERVER_HIGHSCORE_ENTRY_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_ENTRY_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/score.hpp>
#include <fruitserver/ascii/string.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
class highscore_entry
{
public:
	explicit
	highscore_entry();

	explicit
	highscore_entry(
		ascii::string const &name,
		fruitserver::score,
		ascii::string const &datetime);

	ascii::string const &
	name() const;

	fruitserver::score
	score() const;

	ascii::string const &
	datetime() const;

	bool
	operator==(
		highscore_entry const &) const;
private:
	ascii::string name_;
	fruitserver::score score_;
	ascii::string datetime_;
};
}

#endif
