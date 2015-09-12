#ifndef FRUITAPP_GUI_DIALOGS_INGAME_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_INGAME_MENU_HPP_INCLUDED

#include <fcppt/function_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
class ingame_menu
{
FCPPT_NONCOPYABLE(
	ingame_menu);
public:
	typedef
	fcppt::function<void ()>
	game_callback;

	typedef
	fcppt::function<void ()>
	main_menu_callback;

	typedef
	fcppt::function<void ()>
	quit_callback;

	virtual fcppt::signal::auto_connection
	register_game_callback(
		game_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_main_menu_callback(
		main_menu_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_quit_callback(
		quit_callback const &) = 0;

	virtual
	~ingame_menu() = 0;
protected:
	ingame_menu();
};
}
}
}

#endif
