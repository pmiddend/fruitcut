#include <fruitapp/gui/ce/table/column.hpp>
#include <fcppt/string.hpp>


fruitapp::gui::ce::table::column::column(
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
fruitapp::gui::ce::table::column::name() const
{
	return
		name_;
}

fruitapp::gui::ce::table::column::width_type
fruitapp::gui::ce::table::column::width() const
{
	return
		width_;
}
