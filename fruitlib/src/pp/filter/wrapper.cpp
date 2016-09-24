#include <fruitlib/pp/filter/wrapper.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/assert/unimplemented_message.hpp>


fruitlib::pp::filter::wrapper::wrapper()
:
	filter_(
		nullptr
	),
	name_(
		FCPPT_TEXT("WHAT HAPPEN!?")),
	active_(
		true)
{
}

fruitlib::pp::filter::wrapper::wrapper(
	wrapper const &_wrapper)
:
	filter_(
		_wrapper.filter_),
	name_(
		_wrapper.name_),
	active_(
		_wrapper.active_)
{
}

fruitlib::pp::filter::wrapper::wrapper(
	fruitlib::pp::filter::base &_filter,
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

fruitlib::pp::filter::wrapper &
fruitlib::pp::filter::wrapper::operator=(
	wrapper const &_wrapper)
{
	filter_ = _wrapper.filter_;
	name_ = _wrapper.name_;
	active_ = _wrapper.active_;
	return *this;
}

fruitlib::pp::filter::base &
fruitlib::pp::filter::wrapper::filter() const
{
	FCPPT_ASSERT_ERROR(
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

fcppt::string const &
fruitlib::pp::filter::wrapper::name() const
{
	return name_;
}

