#ifndef FRUITAPP_GUI_CE_GET_MODEL_HPP_INCLUDED
#define FRUITAPP_GUI_CE_GET_MODEL_HPP_INCLUDED

#include <fruitapp/gui/ce/table/column_sequence.hpp>
#include <fruitapp/gui/ce/table/model.hpp>
#include <fruitapp/gui/ce/table/row_added.hpp>
#include <fruitapp/gui/ce/table/row_added_fn.hpp>
#include <fruitapp/gui/ce/table/row_removed.hpp>
#include <fruitapp/gui/ce/table/row_removed_fn.hpp>
#include <fruitapp/highscore/entry_set.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace fruitapp
{
namespace gui
{
namespace ce
{
class get_model
:
	public fruitapp::gui::ce::table::model
{
FCPPT_NONCOPYABLE(
	get_model);
public:
	get_model();

	fruitapp::gui::ce::table::column_sequence const
	columns() const;

	fcppt::signal::auto_connection
	row_added(
		fruitapp::gui::ce::table::row_added const &);

	fcppt::signal::auto_connection
	row_removed(
		fruitapp::gui::ce::table::row_removed const &);

	void
	reset(
		fruitapp::highscore::entry_set const &);

	~get_model();
private:
	fcppt::signal::object<fruitapp::gui::ce::table::row_added_fn> row_added_;
	fcppt::signal::object<fruitapp::gui::ce::table::row_removed_fn> row_removed_;
	fruitapp::highscore::entry_set current_entry_set_;
};
}
}
}

#endif
