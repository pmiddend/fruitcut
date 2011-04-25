#ifndef FRUITCUT_SCENIC_NODES_IMMORTAL_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_IMMORTAL_HPP_INCLUDED

#include "with_lifetime.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class immortal
:
	public virtual with_lifetime
{
FCPPT_NONCOPYABLE(
	immortal);
public:
	bool 
	dead() const;
private:
	explicit	
	immortal();
};
}
}
}

#endif
