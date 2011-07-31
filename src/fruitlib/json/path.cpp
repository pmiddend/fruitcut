#include <fruitlib/json/path.hpp>
#include <iostream>

fruitlib::json::path::path(
	const_iterator const _begin,
	const_iterator const _end)
:
	sequence_(
		_begin,
		_end)
{
}

fruitlib::json::path::path(
	fcppt::string const &s)
:
	sequence_()
{
	sequence_.push_back(
		s);
}

fruitlib::json::path::const_iterator 
fruitlib::json::path::begin() const
{
	return sequence_.begin();
}

fruitlib::json::path::const_iterator 
fruitlib::json::path::end() const
{
	return sequence_.end();
}

fruitlib::json::path::size_type
fruitlib::json::path::size() const
{
	return sequence_.size();
}

fruitlib::json::path::const_reference 
fruitlib::json::path::back() const
{
	return sequence_.back();
}

bool
fruitlib::json::path::empty() const
{
	return sequence_.empty();
}

fruitlib::json::path &
fruitlib::json::path::operator/(
	fcppt::string const &s)
{
	sequence_.push_back(
		s);
	return *this;
}
