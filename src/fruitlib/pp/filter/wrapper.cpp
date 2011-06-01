#include "wrapper.hpp"
#include <fcppt/assert.hpp>

fruitcut::fruitlib::pp::filter::wrapper::wrapper()
:
	filter_(
		0),
	name_(
		FCPPT_TEXT("WHAT HAPPEN!?")),
	active_(
		true)
{
}

fruitcut::fruitlib::pp::filter::wrapper::wrapper(
	base &_filter,
	fcppt::string const &_name,
	bool const _active)
:
	filter_(
		&_filter),
	name_(
		_name),
	active_(
		_active)
{
}

fruitcut::fruitlib::pp::filter::base &
fruitcut::fruitlib::pp::filter::wrapper::filter() const
{
	FCPPT_ASSERT(
		filter_);
	return *filter_;
}

bool 
fruitcut::fruitlib::pp::filter::wrapper::active() const
{
	return active_;
}

void 
fruitcut::fruitlib::pp::filter::wrapper::active(
	bool const _active)
{
	active_ = _active;
}

fcppt::string const
fruitcut::fruitlib::pp::filter::wrapper::name() const
{
	return name_;
}

