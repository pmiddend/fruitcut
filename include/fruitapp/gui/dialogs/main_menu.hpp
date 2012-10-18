#ifndef FRUITAPP_GUI_DIALOGS_MAIN_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_MAIN_MENU_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
class main_menu
{
FCPPT_NONCOPYABLE(
	main_menu);
public:
	typedef
	fcppt::function::object<void ()>
	start_callback;

	typedef
	fcppt::function::object<void ()>
	settings_callback;

	typedef
	fcppt::function::object<void ()>
	highscore_callback;

	typedef
	fcppt::function::object<void ()>
	quit_callback;

	virtual fcppt::signal::auto_connection
	register_quit_callback(
		quit_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_settings_callback(
		settings_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_start_callback(
		start_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_highscore_callback(
		highscore_callback const &) = 0;

	virtual
	~main_menu() = 0;
protected:
	main_menu();
};
}
}
}

#endif
