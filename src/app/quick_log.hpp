#ifndef FRUITCUT_APP_QUICK_LOG_HPP_INCLUDED
#define FRUITCUT_APP_QUICK_LOG_HPP_INCLUDED

#include "../fruitlib/scenic/nodes/intrusive_group.hpp"
#include "../fruitlib/font/intrusive_scene_node.hpp"
#include "../fruitlib/font/cache_fwd.hpp"
#include <sge/renderer/scalar.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/font/text/string.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <deque>

namespace fruitcut
{
namespace app
{
class quick_log
:
	public fruitlib::scenic::nodes::intrusive_group
{
FCPPT_NONCOPYABLE(
	quick_log);
public:
	explicit
	quick_log(
		sge::parse::json::object const &,
		fruitlib::font::cache &,
		sge::viewport::manager &,
		sge::renderer::device const &);

	// Take fcppt::string instead of font::text::string here for
	// convenience
	void
	add_message(
		fcppt::string const &);
private:
	typedef
	fcppt::math::dim::static_<sge::renderer::scalar,2>::type
	fractional_dimension;

	// There's no _real_ reason for this being a deque, it's just cool
	// to use it.
	typedef
	std::deque<sge::font::text::string>
	message_sequence;

	fruitlib::font::intrusive_scene_node font_node_;
	fractional_dimension fractional_size_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	sge::time::timer message_delete_timer_;
	message_sequence messages_; 

	void
	viewport_change(
		sge::renderer::device const &);
	
	void
	update();
};
}
}

#endif
