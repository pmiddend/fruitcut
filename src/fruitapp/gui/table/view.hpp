#ifndef FRUITAPP_GUI_TABLE_VIEW_HPP_INCLUDED
#define FRUITAPP_GUI_TABLE_VIEW_HPP_INCLUDED

#include "model_fwd.hpp"
#include "row.hpp"
#include "row_index.hpp"
#include <sge/charconv/system_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace CEGUI
{
class Window;
class MultiColumnList;
}

namespace fruitapp
{
namespace gui
{
namespace table
{
class view
{
FCPPT_NONCOPYABLE(
	view);
public:
	explicit
	view(
		sge::charconv::system &,
		CEGUI::Window &,
		table::model &);

	~view();
private:
	sge::charconv::system &charconv_system_;
	CEGUI::MultiColumnList &impl_;
	table::model &model_;
	fcppt::signal::scoped_connection row_added_connection_;
	fcppt::signal::scoped_connection row_removed_connection_;

	void
	row_added(
		table::row_index::value_type const &,
		table::row const &);

	void
	row_removed(
		table::row_index::value_type const &);
};
}
}
}

#endif
