#ifndef FRUITCUT_EVENTS_TICK_HPP_INCLUDED
#define FRUITCUT_EVENTS_TICK_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
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
		sge::renderer::scalar);

	sge::renderer::scalar
	time_delta() const;
private:
	sge::renderer::scalar time_delta_;
};
}
}

#endif
