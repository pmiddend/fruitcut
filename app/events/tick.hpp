#ifndef FRUITCUT_APP_EVENTS_TICK_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_TICK_HPP_INCLUDED

#include <sge/time/funit.hpp>
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
		sge::time::funit);

	sge::time::funit
	delta() const;
private:
	sge::time::funit delta_;
};
}
}
}

#endif
