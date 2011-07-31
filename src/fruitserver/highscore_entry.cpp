#include <fruitserver/highscore_entry.hpp>
#include <string>

fruitserver::highscore_entry::highscore_entry()
:
	name_(),
	score_(),
	datetime_()
{
}

fruitserver::highscore_entry::highscore_entry(
	ascii::string const &_name,
	fruitserver::score const  _score,
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

fruitserver::ascii::string const &
fruitserver::highscore_entry::name() const
{
	return 
		name_;
}

fruitserver::score
fruitserver::highscore_entry::score() const
{
	return score_;
}

fruitserver::ascii::string const & 
fruitserver::highscore_entry::datetime() const
{
	return datetime_;
}

bool
fruitserver::highscore_entry::operator==(
	highscore_entry const &r) const
{
	return 
		name_ == r.name_ &&
		score_ == r.score_ && 
		datetime_ == r.datetime_;
}
