#ifndef FRUITAPP_POSTPROCESSING_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SYSTEM_HPP_INCLUDED

#include <fruitapp/postprocessing/render_callback.hpp>
#include <fruitapp/postprocessing/subsystems/main_unique_ptr.hpp>
#include <fruitapp/postprocessing/subsystems/paused_unique_ptr.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace postprocessing
{
class system
{
FCPPT_NONCOPYABLE(
	system);
public:
	virtual
	fruitapp::postprocessing::subsystems::main_unique_ptr
	create_main_subsystem(
		fruitlib::scenic::optional_parent const &,
		fruitapp::postprocessing::render_callback const &) = 0;

	virtual
	fruitapp::postprocessing::subsystems::paused_unique_ptr
	create_paused_subsystem(
		fruitlib::scenic::optional_parent const &) = 0;

	virtual
	~system() = 0;
protected:
	system();
};
}
}

#endif
