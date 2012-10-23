#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_RANKING_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_RANKING_HPP_INCLUDED

#include <fruitapp/gui/dialogs/ranking.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>


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
		fruitapp::gui::dialogs::ranking::highscore_callback const &);

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::ranking::main_menu_callback const &);

	fcppt::signal::auto_connection
	register_quit_callback(
		fruitapp::gui::dialogs::ranking::quit_callback const &);

	void
	append_log(
		fcppt::string const &);

	void
	post(
		fruitapp::highscore::name const &,
		fruitapp::highscore::score const &);

	~ranking();
private:
	fcppt::signal::object<void ()> quit_;
	fcppt::signal::object<void ()> highscore_;
	fcppt::signal::object<void ()> main_menu_;
	fcppt::signal::scoped_connection key_connection_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);
};
}
}
}
}

#endif
