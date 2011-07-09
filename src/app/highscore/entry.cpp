#include "entry.hpp"

fruitcut::app::highscore::entry::entry(
	highscore::name const &_name,
	fruitcut::app::score const &_score,
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

fruitcut::app::highscore::name::value_type const &
fruitcut::app::highscore::entry::name() const
{
	return 
		name_;
}

fruitcut::app::score::value_type
fruitcut::app::highscore::entry::score() const
{
	return 
		score_;
}

boost::posix_time::ptime const
fruitcut::app::highscore::entry::date_time() const
{
	return 
		date_time_;
}

bool 
fruitcut::app::highscore::entry::operator<(
	highscore::entry const &right) const
{
	return 
		score_ == right.score_
		?
			date_time_ < right.date_time_
		:
			score_ > right.score_; 
}
