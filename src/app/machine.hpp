#ifndef FRUITCUT_APP_MACHINE_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_HPP_INCLUDED

#include "machine_impl_fwd.hpp"
#include "config_variables_fwd.hpp"
#include "quick_log_fwd.hpp"
#include "states/loading_fwd.hpp"
#include "postprocessing_fwd.hpp"
#include "background_fwd.hpp"
#include "shadow_map_fwd.hpp"
#include "score.hpp"
#include "scene_fwd.hpp"
#include "fruit/prototype_sequence.hpp"
#include "overlay.hpp"
#include "point_sprite/system_node_fwd.hpp"
#include "directional_light_source_fwd.hpp"
#include "../fruitlib/rng_creator_fwd.hpp"
#include "../fruitlib/scenic/nodes/intrusive_group_fwd.hpp"
#include "../fruitlib/audio/sound_controller_fwd.hpp"
#include "../fruitlib/audio/music_controller_fwd.hpp"
#include "../fruitlib/font/cache_fwd.hpp"
#include <sge/time/callback.hpp>
#include <sge/time/time.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/cegui/system_fwd.hpp>
#include <sge/cegui/syringe_fwd.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/event_base.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitcut
{
namespace app
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

	app::config_variables &
	config_variables();

	app::config_variables const &
	config_variables() const;

	void
	run();

	fruitcut::app::postprocessing &
	postprocessing();

	sge::time::callback const 
	timer_callback() const;

	fruitlib::audio::sound_controller &
	sound_controller();

	fruitlib::audio::sound_controller const &
	sound_controller() const;

	fruitlib::audio::music_controller &
	music_controller();

	fruitlib::audio::music_controller const &
	music_controller() const;

	fruitcut::app::background &
	background();

	fruitcut::app::background const &
	background() const;

	fruitcut::app::directional_light_source const &
	main_light_source();

	fruitcut::app::shadow_map &
	shadow_map();

	fruitcut::app::shadow_map const &
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

	fruitlib::rng_creator &
	rng_creator();

	app::score
	last_game_score() const;

	void
	last_game_score(
		app::score const &);

	void
	quit();

	app::scene &
	scene_node();

	app::scene const &
	scene_node() const;

	app::overlay &
	overlay_node();

	app::overlay const &
	overlay_node() const;

	point_sprite::system_node &
	point_sprites();

	point_sprite::system_node const &
	point_sprites() const;

	sge::time::funit
	time_factor() const;

	void
	time_factor(
		sge::time::funit);

	fruitlib::scenic::nodes::intrusive_group &
	root_node();

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

	app::quick_log &
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

	fcppt::scoped_ptr<app::machine_impl> impl_;
	queued_event_list queued_events_;
	bool running_;
};
}
}

#endif
