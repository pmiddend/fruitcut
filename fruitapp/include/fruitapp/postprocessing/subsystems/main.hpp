#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace subsystems
{
class main
{
FCPPT_NONCOPYABLE(
	main);
public:
	virtual void
	render_result(
		sge::renderer::context::core &) = 0;

	virtual void
	loading_progress(
		sge::renderer::scalar) = 0;

	virtual ~main() = 0;
protected:
	main();
};
}
}
}

#endif
