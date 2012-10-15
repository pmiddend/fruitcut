#ifndef FRUITAPP_GUI_BUTTON_HPP_INCLUDED
#define FRUITAPP_GUI_BUTTON_HPP_INCLUDED

#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
#include <CEGUI/Window.h>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
class button
{
FCPPT_NONCOPYABLE(
	button);
public:
	typedef
	void
	push_callback_type();

	typedef
	fcppt::function::object<push_callback_type>
	push_callback_function;

	explicit
	button(
		fruitlib::audio::sound_controller &,
 		CEGUI::Window &);

	fcppt::signal::auto_connection
	push_callback(
		push_callback_function const &);
private:
	fruitlib::audio::sound_controller &sound_controller_;
	CEGUI::Event::ScopedConnection push_connection_;
	CEGUI::Event::ScopedConnection hover_connection_;
	fcppt::signal::object<push_callback_type> push_signal_;

	bool
	internal_push_callback(
		CEGUI::EventArgs const &);

	bool
	internal_hover_callback(
		CEGUI::EventArgs const &);
};
}
}

#endif
