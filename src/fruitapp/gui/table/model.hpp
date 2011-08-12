#ifndef FRUITAPP_GUI_TABLE_MODEL_HPP_INCLUDED
#define FRUITAPP_GUI_TABLE_MODEL_HPP_INCLUDED

#include <fruitapp/gui/table/row_added.hpp>
#include <fruitapp/gui/table/row_removed.hpp>
#include <fruitapp/gui/table/column_sequence.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>

namespace fruitapp
{
namespace gui
{
namespace table
{
class model
{
FCPPT_NONCOPYABLE(
	model);
public:
	virtual table::column_sequence const
	columns() const = 0;

	virtual fcppt::signal::auto_connection
	row_added(
		table::row_added const &) = 0;

	virtual fcppt::signal::auto_connection
	row_removed(
		table::row_removed const &) = 0;

	virtual ~model();
protected:
	explicit
	model();
};
}
}
}

#endif