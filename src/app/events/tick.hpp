#ifndef FRUITCUT_APP_EVENTS_TICK_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_TICK_HPP_INCLUDED

#include <sge/time/duration.hpp>
#include <sge/time/funit.hpp>
#include <fcppt/chrono/duration.hpp>
#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
class tick
:
	public boost::statechart::event<tick>
{
public:
	explicit
	tick(
		sge::time::duration const &);

	sge::time::funit
	delta_ms() const;

	sge::time::duration const &
	delta() const;
private:
	sge::time::duration delta_;
};
}
}
}

#endif
