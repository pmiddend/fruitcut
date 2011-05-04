#ifndef FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include "system.hpp"
#include "base.hpp"
#include "unique_base_ptr.hpp"
#include "../../scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
class system_node
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	system_node);
public:
	explicit
	system_node(
		sge::renderer::device &);

	void
	push_back(
		unique_base_ptr);

	~system_node();
private:
	typedef
	boost::ptr_list<point_sprite::base>
	child_sequence;

	system system_;
	child_sequence children_;

	void
	update();

	void
	render();
};
}
}
}

#endif
