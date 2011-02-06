#ifndef FRUITCUT_PARTICLE_OBJECTS_BASE_HPP_INCLUDED
#define FRUITCUT_PARTICLE_OBJECTS_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace particle
{
namespace objects
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	explicit
	base();

	virtual bool
	dead() const = 0;

	virtual void
	update() = 0;

	virtual
	~base();
};
}
}
}

#endif
