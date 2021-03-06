#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fruitapp/gui/dialogs/name_chooser.hpp>
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
class name_chooser
:
	public fruitapp::gui::dialogs::name_chooser
{
FCPPT_NONCOPYABLE(
	name_chooser);
public:
	explicit
	name_chooser(
		sge::input::keyboard::device &
	);

	fcppt::signal::auto_connection
	register_submit_callback(
		fruitapp::gui::dialogs::name_chooser::submit_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::name_chooser::main_menu_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_restart_callback(
		fruitapp::gui::dialogs::name_chooser::restart_callback const &
	)
	override;

	fcppt::string
	name() const
	override;

	void
	name(
		fcppt::string const &
	)
	override;

	~name_chooser()
	override;
private:
	fcppt::signal::object<void ()> submit_;
	fcppt::signal::object<void ()> main_menu_;
	fcppt::signal::object<void ()> restart_;
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
