#ifndef FRUITAPP_STATES_MENU_SETTINGS_HPP_INCLUDED
#define FRUITAPP_STATES_MENU_SETTINGS_HPP_INCLUDED

#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/graphics_settings/preset_identifier.hpp>
#include <fruitapp/gui/sound_volume.hpp>
#include <fruitapp/gui/dialogs/settings_unique_ptr.hpp>
#include <fruitapp/states/menu/main_fwd.hpp>
#include <fruitapp/states/menu/superstate.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace states
{
namespace menu
{
class settings
:
	// The second argument has to be a complete type
	public boost::statechart::state<settings,superstate>
{
FCPPT_NONCOPYABLE(
	settings);
public:
	typedef
	boost::mpl::vector1
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	explicit
	settings(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	virtual ~settings();
private:
	fruitapp::gui::dialogs::settings_unique_ptr const settings_;
	fcppt::signal::scoped_connection const main_menu_button_connection_;
	fcppt::signal::scoped_connection const music_volume_connection_;
	fcppt::signal::scoped_connection const effects_volume_connection_;
	fcppt::signal::scoped_connection const quality_connection_;

	void
	music_volume_callback(
		fruitapp::gui::sound_volume const &);

	void
	effects_volume_callback(
		fruitapp::gui::sound_volume const &);

	void
	quality_callback(
		fruitapp::graphics_settings::preset_identifier const &);
};
}
}
}

#endif
