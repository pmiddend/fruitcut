#include <fruitlib/pp/filter/wrapper.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/string.hpp>

fruitlib::pp::filter::wrapper::wrapper()
:
	filter_(
		0),
	name_(
		FCPPT_TEXT("WHAT HAPPEN!?")),
	active_(
		true)
{
}

fruitlib::pp::filter::wrapper::wrapper(
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

fruitlib::pp::filter::base &
fruitlib::pp::filter::wrapper::filter() const
{
	FCPPT_ASSERT(
		filter_);
	return *filter_;
}

bool
fruitlib::pp::filter::wrapper::active() const
{
	return active_;
}

void
fruitlib::pp::filter::wrapper::active(
	bool const _active)
{
	active_ = _active;
}

fcppt::string const
fruitlib::pp::filter::wrapper::name() const
{
	return name_;
}

