#include "scoped_lock.hpp"
#include "object.hpp"

fruitcut::line_drawer::scoped_lock::scoped_lock(
	object &_object)
:
	object_(
		_object)
{
	object_.lock();
}

fruitcut::line_drawer::line_sequence &
fruitcut::line_drawer::scoped_lock::value() const
{
	return object_.lines_;
}

fruitcut::line_drawer::scoped_lock::~scoped_lock()
{
	object_.unlock();
}
