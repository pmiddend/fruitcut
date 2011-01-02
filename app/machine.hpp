#ifndef FRUITCUT_APP_MACHINE_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_HPP_INCLUDED

#include "../particle/system.hpp"
#include "../pp/system.hpp"
#include "../pp/filter/render_to_texture.hpp"
#include "../input/state_manager.hpp"
#include "../input/state.hpp"
#include "states/intro_fwd.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/systems/instance.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/console/object.hpp>
#include <sge/console/gfx.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state_machine.hpp>

namespace fruitcut
{
namespace app
{
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::intro
		>
{
public:
	explicit
	machine(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const;

	sge::systems::instance const &
	systems();

	particle::system &
	particle_system();

	sge::texture::part_ptr const
	create_single_texture(
		fcppt::filesystem::path const &);

	sge::texture::part_ptr const
	create_texture(
		fcppt::filesystem::path const &);

	void
	run();

	~machine();
private:
	sge::parse::json::object const config_file_;
	sge::systems::instance const systems_;
	sge::texture::manager texture_manager_;
	input::state_manager input_manager_;
	input::state console_state_,game_state_;
	sge::console::object console_object_;
	sge::console::gfx console_gfx_;
	pp::system postprocessing_;
	pp::filter::render_to_texture rtt_filter_;
	particle::system particle_system_;
	bool running_;
	fcppt::signal::scoped_connection exit_connection_;
};
}
}

#endif
