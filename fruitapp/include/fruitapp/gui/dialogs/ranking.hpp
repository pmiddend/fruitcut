#ifndef FRUITAPP_GUI_DIALOGS_RANKING_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_RANKING_HPP_INCLUDED

#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/score.hpp>
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
class ranking
{
FCPPT_NONCOPYABLE(
	ranking);
public:
	typedef
	fcppt::function::object<void ()>
	highscore_callback;

	typedef
	fcppt::function::object<void ()>
	main_menu_callback;

	typedef
	fcppt::function::object<void ()>
	quit_callback;

	virtual fcppt::signal::auto_connection
	register_highscore_callback(
		highscore_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_main_menu_callback(
		main_menu_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_quit_callback(
		quit_callback const &) = 0;

	virtual void
	append_log(
		fcppt::string const &) = 0;

	virtual void
	post(
		fruitapp::highscore::name const &,
		fruitapp::highscore::score const &) = 0;

	virtual void
	update();

	virtual
	~ranking() = 0;
protected:
	ranking();
};
}
}
}

#endif
