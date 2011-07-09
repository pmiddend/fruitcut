#include "column.hpp"

fruitcut::app::gui::table::column::column(
	fcppt::string const &_name,
	width_type const _width)
:
	name_(
		_name),
	width_(
		_width)
{
}

fcppt::string const &
fruitcut::app::gui::table::column::name() const
{
	return 
		name_;
}

fruitcut::app::gui::table::column::width_type
fruitcut::app::gui::table::column::width() const
{
	return 
		width_;
}
