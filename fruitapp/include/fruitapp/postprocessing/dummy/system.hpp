#ifndef FRUITAPP_POSTPROCESSING_DUMMY_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_DUMMY_SYSTEM_HPP_INCLUDED

#include <fruitapp/postprocessing/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main_fwd.hpp>
#include <fruitapp/postprocessing/cg/subsystems/paused_fwd.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/target/viewport_fwd.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace dummy
{
class system
:
	public fruitapp::postprocessing::system
{
FCPPT_NONCOPYABLE(
	system);
public:
	system();

	fruitapp::postprocessing::subsystems::main_unique_ptr
	create_main_subsystem(
		fruitlib::scenic::optional_parent const &,
		fruitapp::postprocessing::render_callback const &);

	fruitapp::postprocessing::subsystems::paused_unique_ptr
	create_paused_subsystem(
		fruitlib::scenic::optional_parent const &);

	~system();
private:
	fruitapp::postprocessing::render_callback main_render_callback_;
};
}
}
}

#endif
