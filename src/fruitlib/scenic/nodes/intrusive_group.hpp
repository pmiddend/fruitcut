#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_INTRUSIVE_GROUP_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_INTRUSIVE_GROUP_HPP_INCLUDED

#include "intrusive.hpp"
#include <boost/intrusive/list.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class intrusive_group
:
	public virtual intrusive
{
FCPPT_NONCOPYABLE(
	intrusive_group);
public:
	typedef
	boost::intrusive::list<intrusive,boost::intrusive::constant_time_size<false> >
	child_sequence;

	explicit
	intrusive_group();

	child_sequence const &
	children() const;

	child_sequence &
	children();

	void
	push_front(
		nodes::intrusive &);

	void
	insert_dont_care(
		nodes::intrusive &);

	void
	push_back(
		nodes::intrusive &);

	void
	insert_before(
		nodes::intrusive &new_,
		nodes::intrusive &before);

	void
	insert_after(
		nodes::intrusive &new_,
		nodes::intrusive &before);

	~intrusive_group();

	void
	update();

	void
	render();
private:
	child_sequence children_;
};
}
}
}
}

#endif
