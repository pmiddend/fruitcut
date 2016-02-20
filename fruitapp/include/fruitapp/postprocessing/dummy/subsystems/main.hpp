#ifndef FRUITAPP_POSTPROCESSING_DUMMY_SUBSYSTEMS_MAIN_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_DUMMY_SUBSYSTEMS_MAIN_HPP_INCLUDED

#include <fruitapp/postprocessing/render_callback.hpp>
#include <fruitapp/postprocessing/subsystems/main.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace dummy
{
namespace subsystems
{
class main
:
	public fruitapp::postprocessing::subsystems::main
{
FCPPT_NONCOPYABLE(
	main);
public:
	explicit
	main(
		fruitapp::postprocessing::render_callback const &);

	void
	render_result(
		sge::renderer::context::core &);

	void
	loading_progress(
		sge::renderer::scalar);

	~main();
private:
	fruitapp::postprocessing::render_callback const render_callback_;
};
}
}
}
}

#endif
