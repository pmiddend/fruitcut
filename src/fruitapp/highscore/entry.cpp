#include "entry.hpp"

fruitapp::highscore::entry::entry(
	highscore::name const &_name,
	highscore::score const &_score,
	boost::posix_time::ptime const _date_time)
:
	name_(
		_name.get()),
	score_(
		_score.get()),
	date_time_(
		_date_time)
{
}

fruitapp::highscore::name::value_type const &
fruitapp::highscore::entry::name() const
{
	return 
		name_;
}

fruitapp::highscore::score::value_type
fruitapp::highscore::entry::score() const
{
	return 
		score_;
}

boost::posix_time::ptime const
fruitapp::highscore::entry::date_time() const
{
	return 
		date_time_;
}

bool 
fruitapp::highscore::entry::operator<(
	highscore::entry const &right) const
{
	return 
		score_ == right.score_
		?
			date_time_ < right.date_time_
		:
			score_ > right.score_; 
}
