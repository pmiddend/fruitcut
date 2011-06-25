#include "path.hpp"

fruitcut::fruitlib::json::path::path(
	const_iterator const _begin,
	const_iterator const _end)
:
	sequence_(
		_begin,
		_end)
{
}

fruitcut::fruitlib::json::path::path(
	fcppt::string const &s)
:
	sequence_()
{
	sequence_.push_back(
		s);
}

fruitcut::fruitlib::json::path::const_iterator 
fruitcut::fruitlib::json::path::begin() const
{
	return sequence_.begin();
}

fruitcut::fruitlib::json::path::const_iterator 
fruitcut::fruitlib::json::path::end() const
{
	return sequence_.end();
}

fruitcut::fruitlib::json::path::size_type
fruitcut::fruitlib::json::path::size() const
{
	return sequence_.size();
}

fruitcut::fruitlib::json::path::const_reference 
fruitcut::fruitlib::json::path::back() const
{
	return sequence_.back();
}

bool
fruitcut::fruitlib::json::path::empty() const
{
	return sequence_.empty();
}

fruitcut::fruitlib::json::path &
fruitcut::fruitlib::json::path::operator/(
	fcppt::string const &s)
{
	sequence_.push_back(
		s);
	return *this;
}
