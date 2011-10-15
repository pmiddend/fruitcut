#ifndef FRUITAPP_HIGHSCORE_GET_MODEL_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_GET_MODEL_HPP_INCLUDED

#include <fruitapp/gui/table/model.hpp>
#include <fruitapp/gui/table/row_added.hpp>
#include <fruitapp/gui/table/row_removed.hpp>
#include <fruitapp/gui/table/row_added_fn.hpp>
#include <fruitapp/gui/table/row_removed_fn.hpp>
#include <fruitapp/gui/table/column_sequence.hpp>
#include <fruitapp/highscore/entry_set.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace fruitapp
{
namespace highscore
{
class get_model
:
	public gui::table::model
{
FCPPT_NONCOPYABLE(
	get_model);
public:
	explicit
	get_model();

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

	~get_model();
private:
	fcppt::signal::object<gui::table::row_added_fn> row_added_;
	fcppt::signal::object<gui::table::row_removed_fn> row_removed_;
	highscore::entry_set current_entry_set_;
};
}
}

#endif
