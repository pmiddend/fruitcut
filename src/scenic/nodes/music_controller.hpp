#ifndef FRUITCUT_SCENIC_NODES_MUSIC_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_MUSIC_CONTROLLER_HPP_INCLUDED

#include "intrusive.hpp"
#include "../../audio/music_controller_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class music_controller
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	music_controller);
public:
	explicit
	music_controller(
		audio::music_controller &);

	~music_controller();
private:
	audio::music_controller &music_controller_;

	void
	update();

	void
	render();
};
}
}
}

#endif
