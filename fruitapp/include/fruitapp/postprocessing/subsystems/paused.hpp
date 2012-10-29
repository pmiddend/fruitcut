#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace postprocessing
{
namespace subsystems
{
class paused
{
FCPPT_NONCOPYABLE(
	paused);
public:
	virtual ~paused() = 0;
protected:
	paused();
};
}
}
}

#endif
