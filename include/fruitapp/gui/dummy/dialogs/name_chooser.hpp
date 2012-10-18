#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fruitapp/gui/dialogs/name_chooser.hpp>
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
class name_chooser
:
	public fruitapp::gui::dialogs::name_chooser
{
FCPPT_NONCOPYABLE(
	name_chooser);
public:
	explicit
	name_chooser(
		sge::input::keyboard::device &);

	fcppt::signal::auto_connection
	register_continue_callback(
		fruitapp::gui::dialogs::name_chooser::continue_callback const &);

	fcppt::string const
	name() const;

	~name_chooser();
private:
	fcppt::signal::object<void ()> continue_;
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
