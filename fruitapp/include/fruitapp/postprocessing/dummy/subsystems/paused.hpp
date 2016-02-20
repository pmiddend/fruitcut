#ifndef FRUITAPP_POSTPROCESSING_DUMMY_SUBSYSTEMS_PAUSED_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_DUMMY_SUBSYSTEMS_PAUSED_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/paused.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace dummy
{
namespace subsystems
{
class paused
:
	public fruitapp::postprocessing::subsystems::paused,
	public fruitlib::scenic::node<paused>
{
FCPPT_NONCOPYABLE(
	paused);
public:
	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::render
	>
	scene_reactions;

	explicit
	paused(
		fruitlib::scenic::optional_parent const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	~paused();
};
}
}
}
}

#endif
