#include "entry.hpp"

fruitcut::app::highscore::entry::entry(
	highscore::name const &_name,
	fruitcut::app::score const &_score,
	boost::posix_time::ptime const _date_time)
:
	name(
		_name.get()),
	score_(
		_score.get()),
	date_time_(
		_date_time)
{
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
