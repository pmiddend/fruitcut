#ifndef FRUITCUT_SCENIC_NODES_CURSOR_TRAIL_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_CURSOR_TRAIL_HPP_INCLUDED

#include "intrusive.hpp"
#include "../../cursor_trail_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class cursor_trail
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	cursor_trail);
public:
	explicit
	cursor_trail(
		fruitcut::cursor_trail &);

	~cursor_trail();
private:
	fruitcut::cursor_trail &cursor_trail_;

	void
	update();

	void
	render();
};
}
}
}

#endif
