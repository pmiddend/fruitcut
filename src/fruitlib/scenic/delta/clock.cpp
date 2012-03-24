#include <fruitlib/scenic/delta/clock.hpp>


fruitlib::scenic::delta::clock::clock(
	delta::callback const &_callback)
:
	callback_(
		_callback)
{
}

fruitlib::scenic::delta::clock::~clock()
{
}

void
fruitlib::scenic::delta::clock::update()
{
	now_ +=
		callback_();
}

fruitlib::scenic::delta::clock::time_point
fruitlib::scenic::delta::clock::now() const
{
	return now_;
}
