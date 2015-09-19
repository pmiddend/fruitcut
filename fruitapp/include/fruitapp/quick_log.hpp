#ifndef FRUITAPP_QUICK_LOG_HPP_INCLUDED
#define FRUITAPP_QUICK_LOG_HPP_INCLUDED

#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/font/string.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/static.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <deque>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class quick_log
:
	public fruitlib::scenic::node<quick_log>
{
FCPPT_NONCOPYABLE(
	quick_log);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	quick_log(
		fruitlib::scenic::optional_parent const &,
		sge::parse::json::object const &,
		fruitlib::font::manager &,
		fruitapp::viewport::manager &,
		fruitlib::audio::sound_controller &);

	// Take fcppt::string instead of font::text::string here for
	// convenience
	void
	add_message(
		fcppt::string const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	typedef
	fcppt::math::dim::static_<sge::renderer::scalar,2>
	fractional_dimension;

	// There's no _real_ reason for this being a deque, it's just cool
	// to use it.
	typedef
	std::deque<sge::font::string>
	message_sequence;

	fruitlib::audio::sound_controller &sound_controller_;
	fruitlib::font::scene_node font_node_;
	fractional_dimension fractional_size_;
	sge::timer::basic<sge::timer::clocks::standard> message_delete_timer_;
	message_sequence messages_;
	fcppt::signal::auto_connection viewport_change_connection_;

	void
	viewport_change(
		sge::renderer::target::viewport const &);
};
}

#endif
