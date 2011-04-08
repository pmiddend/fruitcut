#ifndef FRUITCUT_SCENE_NODES_GROUP_HPP_INCLUDED
#define FRUITCUT_SCENE_NODES_GROUP_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace fruitcut
{
namespace scene
{
namespace nodes
{
class group
:
	public base
{
FCPPT_NONCOPYABLE(
	group);
public:
	typedef
	boost::ptr_vector<base>
	child_sequence;

	child_sequence const &
	children() const;

	child_sequence &
	children();

	void
	update();

	void
	render();

	~group();
protected:
	group();
private:
	child_sequence children_;
};
}
}
}

#endif
