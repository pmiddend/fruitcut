#ifndef FRUITAPP_MACHINE_HPP_INCLUDED
#define FRUITAPP_MACHINE_HPP_INCLUDED

#include <fruitapp/machine_impl_fwd.hpp>
#include <fruitapp/config_variables_fwd.hpp>
#include <fruitapp/quick_log_fwd.hpp>
#include <fruitapp/states/loading_fwd.hpp>
#include <fruitapp/postprocessing_fwd.hpp>
#include <fruitapp/background_fwd.hpp>
#include <fruitapp/shadow_map_fwd.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/scene_fwd.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitapp/overlay.hpp>
#include <fruitapp/point_sprite/system_node_fwd.hpp>
#include <fruitapp/directional_light_source_fwd.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/base_fwd.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/audio/music_controller_fwd.hpp>
#include <fruitlib/font/cache_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/cegui/system_fwd.hpp>
#include <sge/cegui/syringe_fwd.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/event_base.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitapp
{
class machine
:
	public boost::statechart::state_machine<machine,states::loading>
{
public:
	typedef
	boost::statechart::state_machine<machine,states::loading>
	base;

	explicit
	machine(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const;

	sge::systems::instance const &
	systems() const;

	sge::model::md3::loader &
	md3_loader();

	fruitapp::config_variables &
	config_variables();

	fruitapp::config_variables const &
	config_variables() const;

	void
	run();

	fruitapp::postprocessing &
	postprocessing();

	fruitapp::ingame_clock const &
	ingame_clock() const;

	fruitlib::audio::sound_controller &
	sound_controller();

	fruitlib::audio::sound_controller const &
	sound_controller() const;

	fruitlib::audio::music_controller &
	music_controller();

	fruitlib::audio::music_controller const &
	music_controller() const;

	fruitapp::background &
	background();

	fruitapp::background const &
	background() const;

	fruitapp::directional_light_source const &
	main_light_source();

	fruitapp::shadow_map &
	shadow_map();

	fruitapp::shadow_map const &
	shadow_map() const;

	sge::camera::object &
	camera();

	sge::camera::object const &
	camera() const;

	fruitlib::font::cache &
	font_cache();

	fruitlib::font::cache const &
	font_cache() const;

	sge::cegui::system &
	gui_system();

	sge::cegui::system const &
	gui_system() const;

	sge::cegui::syringe &
	gui_syringe();

	sge::cegui::syringe const &
	gui_syringe() const;

	fruitlib::random_generator &
	random_generator();

	highscore::score::value_type
	last_game_score() const;

	void
	last_game_score(
		highscore::score::value_type const &);

	void
	quit();

	fruitapp::scene &
	scene_node();

	fruitapp::scene const &
	scene_node() const;

	fruitapp::overlay &
	overlay_node();

	fruitapp::overlay const &
	overlay_node() const;

	point_sprite::system_node &
	point_sprites();

	point_sprite::system_node const &
	point_sprites() const;

	fruitapp::ingame_clock::float_type
	time_factor() const;

	void
	time_factor(
		fruitapp::ingame_clock::float_type);

	fruitlib::scenic::base &
	root_node();

	fruitlib::scenic::delta::callback const
	ingame_clock_callback() const;
	
	fruitlib::scenic::delta::callback const
	standard_clock_callback() const;

	~machine();

	void
	unconsumed_event(
		boost::statechart::event_base const &);

	void
	post_event(
		boost::statechart::event_base const &);

	fruit::prototype_sequence const &
	fruit_prototypes() const;

	fruit::prototype_sequence &
	fruit_prototypes();

	fruitapp::quick_log &
	quick_log();
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
	bool running_;
};
}

#endif
