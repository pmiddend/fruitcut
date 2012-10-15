#ifndef FRUITAPP_GUI_PROGRESS_SLIDER_HPP_INCLUDED
#define FRUITAPP_GUI_PROGRESS_SLIDER_HPP_INCLUDED

#include <fruitapp/gui/button.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace CEGUI
{
class Window;
class ProgressBar;
}

namespace fruitapp
{
namespace gui
{
class progress_slider
{
FCPPT_NONCOPYABLE(
	progress_slider);
public:
	typedef
	float
	value_type;

	typedef
	void
	value_changed_type(
		value_type);

	typedef
	fcppt::function::object<value_changed_type>
	value_changed_function;

	explicit
	progress_slider(
		fruitlib::audio::sound_controller &,
		CEGUI::Window &,
		std::string const &prefix,
		value_type);

	fcppt::signal::auto_connection
	value_changed(
		value_changed_function const &);

	~progress_slider();
private:
	fcppt::signal::object<value_changed_type> value_changed_signal_;
	CEGUI::ProgressBar &progress_window_;
	gui::button increase_,decrease_;
	fcppt::signal::scoped_connection increase_connection_;
	fcppt::signal::scoped_connection decrease_connection_;

	void
	increase_callback();

	void
	decrease_callback();
};
}
}

#endif
