#include "highscore_entry.hpp"
#include "lexical_cast.hpp"
#include <string>

fruitcut::server::highscore_entry::highscore_entry()
:
	name_(),
	score_(),
	score_numerical_()
{
}

fruitcut::server::highscore_entry::highscore_entry(
	std::string const &_name,
	std::string const &_score)
:
	name_(
		_name),
	score_(
		_score),
	score_numerical_(
		server::lexical_cast<unsigned>(
			_score))
{
}

std::string const &
fruitcut::server::highscore_entry::name() const
{
	return 
		name_;
}

std::string const & 
fruitcut::server::highscore_entry::score() const
{
	return score_;
}

unsigned 
fruitcut::server::highscore_entry::score_numerical() const
{
	return score_numerical_;
}

bool
fruitcut::server::highscore_entry::operator==(
	highscore_entry const &r) const
{
	return 
		name_ == r.name_ &&
		score_ == r.score_;
}
