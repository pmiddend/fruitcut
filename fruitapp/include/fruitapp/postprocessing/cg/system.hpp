#ifndef FRUITAPP_POSTPROCESSING_CG_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_CG_SYSTEM_HPP_INCLUDED

#include <fruitapp/postprocessing/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main_fwd.hpp>
#include <fruitapp/postprocessing/cg/subsystems/paused_fwd.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/target/viewport_fwd.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/reference.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace cg
{
class system
:
	public fruitapp::postprocessing::system
{
FCPPT_NONCOPYABLE(
	system);
public:
	system(
		sge::shader::context &,
		fruitapp::viewport::manager &,
		sge::parse::json::object const &);

	fruitapp::postprocessing::subsystems::main_unique_ptr
	create_main_subsystem(
		fruitlib::scenic::optional_parent const &,
		fruitapp::postprocessing::render_callback const &);

	fruitapp::postprocessing::subsystems::paused_unique_ptr
	create_paused_subsystem(
		fruitlib::scenic::optional_parent const &);

	~system();
private:
	friend class fruitapp::postprocessing::cg::subsystems::main;
	friend class fruitapp::postprocessing::cg::subsystems::paused;

	typedef
	fcppt::optional::reference<fruitapp::postprocessing::cg::subsystems::main>
	optional_main_system;

	sge::parse::json::object const &configuration_;
	fruitlib::pp::texture::manager texture_manager_;
	fruitlib::pp::filter::manager filter_manager_;
	fcppt::signal::auto_connection const viewport_change_connection_;
	optional_main_system main_system_;
	bool pause_active_;

	void
	viewport_change(
		sge::renderer::target::viewport const &);

	void
	main_destroyed();

	void
	paused_destroyed();

	fruitlib::pp::texture::counted_instance const
	main_result_texture();
};
}
}
}

#endif
