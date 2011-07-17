#include "highscore_entry.hpp"
#include <string>

fruitcut::server::highscore_entry::highscore_entry()
:
	name_(),
	score_(),
	datetime_()
{
}

fruitcut::server::highscore_entry::highscore_entry(
	ascii::string const &_name,
	server::score const  _score,
	ascii::string const &_datetime)
:
	name_(
		_name),
	score_(
		_score),
	datetime_(
		_datetime)
{
}

fruitcut::server::ascii::string const &
fruitcut::server::highscore_entry::name() const
{
	return 
		name_;
}

fruitcut::server::score
fruitcut::server::highscore_entry::score() const
{
	return score_;
}

fruitcut::server::ascii::string const & 
fruitcut::server::highscore_entry::datetime() const
{
	return datetime_;
}

bool
fruitcut::server::highscore_entry::operator==(
	highscore_entry const &r) const
{
	return 
		name_ == r.name_ &&
		score_ == r.score_ && 
		datetime_ == r.datetime_;
}
