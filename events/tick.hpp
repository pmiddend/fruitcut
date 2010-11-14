#ifndef FRUITCUT_EVENTS_TICK_HPP_INCLUDED
#define FRUITCUT_EVENTS_TICK_HPP_INCLUDED

#include "../scalar.hpp"
#include <boost/statechart/event.hpp>

namespace fruitcut
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
		scalar);

	scalar
	time_delta() const;
private:
	scalar time_delta_;
};
}
}

#endif
