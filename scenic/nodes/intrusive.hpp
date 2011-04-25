#ifndef FRUITCUT_SCENIC_NODES_INTRUSIVE_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_INTRUSIVE_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/noncopyable.hpp>
#include <boost/intrusive/list.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class intrusive
:
	public virtual base,
	public 
		boost::intrusive::list_base_hook
		<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink>
		>
{
FCPPT_NONCOPYABLE(
	intrusive);
protected:
	explicit
	intrusive();

	~intrusive();
};
}
}
}

#endif
