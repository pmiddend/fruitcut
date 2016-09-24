#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_RANKING_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_RANKING_HPP_INCLUDED

#include <fruitapp/gui/dialogs/ranking.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>


namespace fruitapp
{
namespace gui
{
namespace dummy
{
namespace dialogs
{
class ranking
:
	public fruitapp::gui::dialogs::ranking
{
FCPPT_NONCOPYABLE(
	ranking);
public:
	explicit
	ranking(
		sge::input::keyboard::device &);

	fcppt::signal::auto_connection
	register_highscore_callback(
		fruitapp::gui::dialogs::ranking::highscore_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::ranking::main_menu_callback const &
	)
	override;

	void
	append_log(
		fcppt::string const &
	)
	override;

	void
	post(
		fruitapp::highscore::name const &,
		fruitapp::highscore::score const &
	)
	override;

	~ranking()
	override;
private:
	fcppt::signal::object<void ()> highscore_;
	fcppt::signal::object<void ()> main_menu_;
	fcppt::signal::auto_connection key_connection_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);
};
}
}
}
}

#endif
