#ifndef FRUITCUT_APP_SCENE_HPP_INCLUDED
#define FRUITCUT_APP_SCENE_HPP_INCLUDED

#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/parent_fwd.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
class scene
:
	public fruitlib::scenic::node<scene>
{
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	scene(
		fruitlib::scenic::parent const &);

	void
	active(
		bool);

	bool
	active() const;

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	render_children();
private:
	bool active_;
};
}
}

#endif
