#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_GROUP_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_GROUP_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
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
	boost::ptr_list<base>
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
}

#endif
