#ifndef FRUITAPP_MACHINE_HPP_INCLUDED
#define FRUITAPP_MACHINE_HPP_INCLUDED

#include <fruitapp/config_variables_fwd.hpp>
#include <fruitapp/directional_light_source_fwd.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/machine_impl_fwd.hpp>
#include <fruitapp/overlay.hpp>
#include <fruitapp/quick_log_fwd.hpp>
#include <fruitapp/scene_fwd.hpp>
#include <fruitapp/systems.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitapp/gui/system_fwd.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/point_sprite/system_node_fwd.hpp>
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <fruitapp/postprocessing/system_fwd.hpp>
#include <fruitapp/postprocessing/subsystems/main_fwd.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitapp/shadow_map/optional_object_ref.hpp>
#include <fruitapp/states/loading_fwd.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/audio/music_controller_fwd.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/font/cache_fwd.hpp>
#include <fruitlib/scenic/base_fwd.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <awl/main/exit_code.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/event_base.hpp>
#include <boost/statechart/state_machine.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class machine
:
	public boost::statechart::state_machine<machine,states::loading>
{
FCPPT_NONCOPYABLE(
	machine);
public:
	typedef
	boost::statechart::state_machine<machine,states::loading>
	base;

	machine(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const
	FCPPT_PP_CONST;

	fruitapp::systems const &
	systems() const
	FCPPT_PP_CONST;

	sge::shader::optional_context_ref const
	shader_context() const;

	sge::model::md3::loader &
	md3_loader()
	FCPPT_PP_CONST;

	fruitapp::config_variables &
	config_variables()
	FCPPT_PP_CONST;

	fruitapp::config_variables const &
	config_variables() const
	FCPPT_PP_CONST;

	awl::main::exit_code const
	run()
	FCPPT_PP_WARN_UNUSED_RESULT;

	fruitapp::postprocessing::system &
	postprocessing_system()
	FCPPT_PP_CONST;

	fruitapp::postprocessing::subsystems::main &
	postprocessing_main()
	FCPPT_PP_CONST;

	fruitapp::ingame_clock const &
	ingame_clock() const
	FCPPT_PP_CONST;

	fruitlib::audio::sound_controller &
	sound_controller()
	FCPPT_PP_CONST;

	fruitlib::audio::sound_controller const &
	sound_controller() const
	FCPPT_PP_CONST;

	fruitlib::audio::music_controller &
	music_controller()
	FCPPT_PP_CONST;

	fruitlib::audio::music_controller const &
	music_controller() const
	FCPPT_PP_CONST;

	fruitapp::directional_light_source const &
	main_light_source()
	FCPPT_PP_CONST;

	fruitapp::shadow_map::optional_object_ref const
	shadow_map()
	FCPPT_PP_CONST;

	sge::camera::first_person::object &
	camera()
	FCPPT_PP_CONST;

	sge::camera::first_person::object const &
	camera() const
	FCPPT_PP_CONST;

	fruitlib::font::cache &
	font_cache()
	FCPPT_PP_CONST;

	fruitlib::font::cache const &
	font_cache() const
	FCPPT_PP_CONST;

	fruitapp::gui::system &
	gui_system();


	fruitlib::random_generator &
	random_generator()
	FCPPT_PP_CONST;

	highscore::score::value_type
	last_game_score() const
	FCPPT_PP_CONST;

	void
	last_game_score(
		highscore::score::value_type const &);

	void
	quit(
		awl::main::exit_code);

	fruitapp::scene &
	scene_node()
	FCPPT_PP_CONST;

	fruitapp::scene const &
	scene_node() const
	FCPPT_PP_CONST;

	fruitapp::overlay &
	overlay_node()
	FCPPT_PP_CONST;

	fruitapp::overlay const &
	overlay_node() const
	FCPPT_PP_CONST;

	point_sprite::system_node &
	point_sprites()
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_CONST;

	point_sprite::system_node const &
	point_sprites() const
	FCPPT_PP_CONST;

	fruitapp::ingame_clock::float_type
	time_factor() const
	FCPPT_PP_CONST;

	sge::renderer::texture::emulate_srgb::type
	emulate_srgb() const;

	void
	time_factor(
		fruitapp::ingame_clock::float_type);

	fruitlib::scenic::base &
	root_node()
	FCPPT_PP_CONST;

	fruitlib::scenic::delta::callback const
	ingame_clock_callback() const
	FCPPT_PP_CONST;

	fruitlib::scenic::delta::callback const
	standard_clock_callback() const
	FCPPT_PP_CONST;

	~machine();

	void
	unconsumed_event(
		boost::statechart::event_base const &);

	void
	post_event(
		boost::statechart::event_base const &);

	fruit::prototype_sequence const &
	fruit_prototypes() const
	FCPPT_PP_CONST;

	fruit::prototype_sequence &
	fruit_prototypes()
	FCPPT_PP_CONST;

	fruitapp::quick_log &
	quick_log()
	FCPPT_PP_CONST;

	fruitapp::viewport::manager &
	viewport_manager()
	FCPPT_PP_CONST;

	fruitapp::projection_manager::object &
	projection_manager()
	FCPPT_PP_CONST;
private:
	/*
		This part needs some explanation. What are we doing here?

		The problem is that we've got two trees at the same time: The
		statechart state tree and the scene graph (technically a graph,
		but it's really a tree). Each frame, the scene tree is traversed
		and the game takes its course.

		Every once in a while, however, the game's state changes. For
		example, it might change from "running" to "paused" or (more
		drastically) from "running" to "gameover". This happens, as almost
		everything, while traversing the scene tree, so in a loop like this:

		foreach(child_node c,children())
			c->update();

		Changing the game state is, however, destructive! Thus, if a
		transition occurs inside one of these update calls, it might kill
		the node's parent or even the parent node's parent! Hacking this
		is very risky, so I'm taking another approach: Defer all the
		events until after the scene tree is traversed.

		At first I though this was fairly easy, since
		boost::statechart::state_machine already has a list of "queued
		events", to which new events can be added with a call to
		machine::post_event. Alas, you cannot simply say
		"process_queued_events" (this method exists, but it is
		private). So we need the code below to build our own queue and
		then transfer this queue to statechart (a call to "process_event"
		starts the queue processing).
	*/
	typedef
	std::allocator<void>
	allocator;

	// Can't call it "event_base_ptr_type" since that's "reserved" by
	// statechart
	typedef
	boost::intrusive_ptr<boost::statechart::event_base const>
	my_event_base_ptr_type;

	typedef
	std::list
	<
		my_event_base_ptr_type,
		boost::detail::allocator::rebind_to
		<
			allocator,
			my_event_base_ptr_type
		>::type
	>
	queued_event_list;

	fcppt::scoped_ptr<fruitapp::machine_impl> impl_;
	queued_event_list queued_events_;
};
}

#endif
