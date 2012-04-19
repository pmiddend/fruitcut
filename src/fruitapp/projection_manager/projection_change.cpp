#include <fruitapp/projection_manager/projection_change.hpp>

fruitapp::projection_manager::projection_change::projection_change(
	fruitlib::perspective_projection_information const &_perspective_projection_information)
:
	perspective_projection_information_(
		_perspective_projection_information)
{
}

fruitlib::perspective_projection_information const &
fruitapp::projection_manager::projection_change::perspective_projection_information() const
{
	return
		perspective_projection_information_;
}

fruitapp::projection_manager::projection_change::~projection_change()
{
}
