#ifndef FRUITAPP_GUI_DIALOGS_SETTINGS_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_SETTINGS_HPP_INCLUDED

#include <fruitapp/gui/sound_volume.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
class settings
{
FCPPT_NONCOPYABLE(
	settings);
public:
	typedef
	void
	volume_change_function(
		fruitapp::gui::sound_volume const &);

	typedef
	fcppt::function::object<volume_change_function>
	volume_change_callback;

	typedef
	fcppt::function::object<void ()>
	back_callback;

	virtual fcppt::signal::auto_connection
	register_effects_volume_change_callback(
		volume_change_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_music_volume_change_callback(
		volume_change_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_back_callback(
		back_callback const &) = 0;

	virtual
	~settings() = 0;
protected:
	settings();
};
}
}
}

#endif
