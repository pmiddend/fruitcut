#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_SOUND_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_SOUND_CONTROLLER_HPP_INCLUDED

#include "intrusive.hpp"
#include "../../audio/sound_controller_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class sound_controller
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	sound_controller);
public:
	explicit
	sound_controller(
		audio::sound_controller &);

	~sound_controller();
private:
	audio::sound_controller &sound_controller_;

	void
	update();

	void
	render();
};
}
}
}
}

#endif
