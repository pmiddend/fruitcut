#ifndef FRUITAPP_CURSOR_MANAGER_HPP_INCLUDED
#define FRUITAPP_CURSOR_MANAGER_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/cursor/instance_fwd.hpp>
#include <fruitapp/cursor/instance_sequence_unique_ptr.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/input/processor_fwd.hpp>
#include <sge/input/cursor/discover_event_fwd.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/cursor/remove_event_fwd.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <unordered_set>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace cursor
{
/**
\brief Takes care of cursor discover/remove events even if we're not ingame
*/
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	manager(
		sge::input::processor &,
		sge::renderer::device::ffp &,
		sge::renderer::target::base const &,
		sge::camera::base const &,
		fruitlib::texture_manager &,
		fruitapp::viewport::manager &,
		fruitlib::audio::sound_controller &,
		fruitapp::ingame_clock const &,
		sge::parse::json::object const &);

	fruitapp::cursor::instance_sequence_unique_ptr
	create_instances(
		fruitlib::scenic::optional_parent const &);

	~manager();
private:
	friend class fruitapp::cursor::instance;
	friend class fruitapp::cursor::instance_sequence;

	typedef
	std::unordered_set<
		sge::input::cursor::object *
	>
	cursor_sequence;

	typedef
	std::unordered_set<
		fruitapp::cursor::instance_sequence *
	>
	instance_sequence_sequence;

	sge::renderer::device::ffp &renderer_;
	sge::renderer::target::base const &target_;
	sge::camera::base const &camera_;
	fruitlib::texture_manager &texture_manager_;
	fruitapp::viewport::manager &viewport_manager_;
	fruitlib::audio::sound_controller &sound_controller_;
	fruitapp::ingame_clock const &ingame_clock_;
	sge::parse::json::object const configuration_;
	fcppt::signal::scoped_connection const discover_connection_;
	fcppt::signal::scoped_connection const remove_connection_;
	cursor_sequence cursors_;
	instance_sequence_sequence instance_sequences_;

	void
	discover(
		sge::input::cursor::discover_event const &);

	void
	remove(
		sge::input::cursor::remove_event const &);

	void
	instance_sequence_destroyed(
		fruitapp::cursor::instance_sequence &);
};
}
}

#endif
