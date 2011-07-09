#ifndef FRUITCUT_APP_GUI_TABLE_COLUMN_HPP_INCLUDED
#define FRUITCUT_APP_GUI_TABLE_COLUMN_HPP_INCLUDED

#include <fcppt/string.hpp>

namespace fruitcut
{
namespace app
{
namespace gui
{
namespace table
{
class column
{
public:
	typedef
	float
	width_type;

	explicit
	column(
		fcppt::string const &,
		width_type);

	fcppt::string const &
	name() const;

	width_type
	width() const;
private:
	fcppt::string name_;
	width_type width_;
};
}
}
}
}

#endif
