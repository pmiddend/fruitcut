#ifndef FRUITAPP_GUI_CE_SLIDER_OBJECT_HPP_INCLUDED
#define FRUITAPP_GUI_CE_SLIDER_OBJECT_HPP_INCLUDED

#include <fruitapp/gui/ce/slider/current_value.hpp>
#include <fruitapp/gui/ce/slider/range.hpp>
#include <fruitapp/gui/ce/slider/value_changed_fn.hpp>
#include <fruitapp/gui/ce/slider/value_changed_function.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
#include <fcppt/config/external_end.hpp>


namespace CEGUI
{
class Window;
class Slider;
}

namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace slider
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object(
		fruitlib::audio::sound_controller &,
		CEGUI::Window &,
		fruitapp::gui::ce::slider::range const &,
		fruitapp::gui::ce::slider::current_value const &);

	fcppt::signal::auto_connection
	value_changed(
		fruitapp::gui::ce::slider::value_changed_function const &);

	~object();
private:
	fruitlib::audio::sound_controller &sound_controller_;
	fcppt::signal::object<fruitapp::gui::ce::slider::value_changed_fn> value_changed_signal_;
	CEGUI::Event::ScopedConnection const value_changed_connection_;
	CEGUI::Event::ScopedConnection const value_change_complete_connection_;
	fruitapp::gui::ce::slider::range const range_;

	bool
	internal_value_changed_callback(
		CEGUI::EventArgs const &);

	bool
	internal_value_change_complete_callback(
		CEGUI::EventArgs const &);
};
}
}
}
}

#endif
