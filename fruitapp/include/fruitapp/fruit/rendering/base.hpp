#ifndef FRUITAPP_FRUIT_RENDERING_BASE_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_BASE_HPP_INCLUDED

#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace fruit
{
namespace rendering
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual void
	render(
		sge::renderer::context::core &) = 0;

	virtual ~base() = 0;
protected:
	base();
};
}
}
}

#endif
