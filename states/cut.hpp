#ifndef FRUITCUT_STATES_CUT_HPP_INCLUDED
#define FRUITCUT_STATES_CUT_HPP_INCLUDED

#include "ingame.hpp"
#include "../events/render_overlay.hpp"
#include "../sprite/system.hpp"
#include "../sprite/object.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace states
{
class cut
:
	public boost::statechart::state<cut,ingame>
{
public:
	typedef 
	boost::mpl::vector1
	<
		boost::statechart::custom_reaction<events::render_overlay>
	> 
	reactions;

	explicit
	cut(
		my_context);

	boost::statechart::result
	react(
		events::render_overlay const &);
private:
	sprite::system ss_;
	sprite::object cursor_;
};
}
}

#endif
