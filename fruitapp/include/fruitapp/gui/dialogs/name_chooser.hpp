#ifndef FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>


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
	fcppt::function::object<void ()>
	continue_callback;

	typedef
	fcppt::function::object<void (fcppt::string const &)>
	name_callback;

	virtual fcppt::signal::auto_connection
	register_continue_callback(
		continue_callback const &) = 0;

	virtual fcppt::string const
	name() const = 0;

	virtual
	~name_chooser() = 0;
protected:
	name_chooser();
};
}
}
}

#endif
