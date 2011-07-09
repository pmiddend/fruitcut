#ifndef FRUITCUT_APP_HIGHSCORE_GUI_MODEL_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_GUI_MODEL_HPP_INCLUDED

#include "../gui/table/model.hpp"
#include "../gui/table/row_added.hpp"
#include "../gui/table/row_removed.hpp"
#include "../gui/table/row_added_fn.hpp"
#include "../gui/table/row_removed_fn.hpp"
#include "../gui/table/column_sequence.hpp"
#include "entry_set.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
class gui_model
:
	public gui::table::model
{
FCPPT_NONCOPYABLE(
	gui_model);
public:
	explicit
	gui_model();
	
	gui::table::column_sequence const
	columns() const;

	fcppt::signal::auto_connection
	row_added(
		gui::table::row_added const &);

	fcppt::signal::auto_connection
	row_removed(
		gui::table::row_removed const &);

	void
	reset(
		highscore::entry_set const &);

	~gui_model();
private:
	fcppt::signal::object<gui::table::row_added_fn> row_added_;
	fcppt::signal::object<gui::table::row_removed_fn> row_removed_;
	highscore::entry_set current_entry_set_;
};
}
}
}

#endif
