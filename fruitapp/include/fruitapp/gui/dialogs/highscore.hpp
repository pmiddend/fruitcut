#ifndef FRUITAPP_GUI_DIALOGS_HIGHSCORE_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_HIGHSCORE_HPP_INCLUDED

#include <fruitapp/highscore/entry_set.hpp>
#include <fruitapp/highscore/provider/object_base_fwd.hpp>
#include <fcppt/function_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
class highscore
{
FCPPT_NONCOPYABLE(
	highscore);
public:
	typedef
	fcppt::function<void ()>
	back_callback;

	typedef
	void
	switch_provider_function(
		fruitapp::highscore::provider::object_base &);

	typedef
	fcppt::function<switch_provider_function>
	switch_provider_callback;

	virtual fcppt::signal::auto_connection
	register_back_callback(
		back_callback const &) = 0;

	virtual fcppt::signal::auto_connection
	register_switch_provider_callback(
		switch_provider_callback const &) = 0;

	virtual void
	clear_log() = 0;

	virtual void
	append_log(
		fcppt::string const &) = 0;

	virtual void
	process_list(
		fruitapp::highscore::entry_set const &) = 0;

	virtual
	~highscore() = 0;
protected:
	highscore();
};
}
}
}

#endif
