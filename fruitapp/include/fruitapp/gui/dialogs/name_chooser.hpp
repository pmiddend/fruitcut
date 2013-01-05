#ifndef FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
class name_chooser
{
FCPPT_NONCOPYABLE(
	name_chooser);
public:
	typedef
	std::function<void ()>
	submit_callback;

	typedef
	std::function<void ()>
	main_menu_callback;

	typedef
	std::function<void ()>
	restart_callback;

	typedef
	std::function<void (fcppt::string const &)>
	name_callback;

	virtual fcppt::signal::auto_connection
	register_submit_callback(
		submit_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_main_menu_callback(
		main_menu_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_restart_callback(
		restart_callback const &) = 0;

	virtual fcppt::string const
	name() const = 0;

	virtual void
	name(
		fcppt::string const &) = 0;

	virtual
	~name_chooser() = 0;
protected:
	name_chooser();
};
}
}
}

#endif
