#ifndef FRUITAPP_GUI_CE_TABLE_MODEL_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_MODEL_HPP_INCLUDED

#include <fruitapp/gui/ce/table/column_sequence.hpp>
#include <fruitapp/gui/ce/table/row_added.hpp>
#include <fruitapp/gui/ce/table/row_removed.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace table
{
class model
{
FCPPT_NONCOPYABLE(
	model);
public:
	virtual fruitapp::gui::ce::table::column_sequence const
	columns() const = 0;

	virtual fcppt::signal::auto_connection
	row_added(
		fruitapp::gui::ce::table::row_added const &) = 0;

	virtual fcppt::signal::auto_connection
	row_removed(
		fruitapp::gui::ce::table::row_removed const &) = 0;

	virtual ~model() = 0;
protected:
	model();
};
}
}
}
}

#endif
