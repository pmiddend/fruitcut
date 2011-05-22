#include "superstate.hpp"
#include "running.hpp"
#include "../../events/toggle_pause.hpp"
#include "../../../input/state.hpp"
#include "../../../json/find_member.hpp"
#include "../../../json/parse_color.hpp"
#include "../../../physics/vector3.hpp"
#include "../../../physics/box.hpp"
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <boost/statechart/event_base.hpp>
#include <iostream>

fruitcut::app::states::ingame::superstate::superstate(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&machine::process_event,
					&context<machine>(),
					events::toggle_pause())))),
	physics_world_(
		// The box is ignored for now
		physics::box(),
		json::find_member<physics::vector3>(
			context<machine>().config_file(),
			FCPPT_TEXT("physics/gravity"))),
	physics_world_node_(
		physics_world_,
		context<machine>().timer_callback()),
	physics_debugger_(
		physics_world_,
		context<machine>().systems().renderer(),
		context<machine>().camera()),
	physics_debugger_node_(
		physics_debugger_),
	physics_debugger_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f3, 
				std::tr1::bind(
					&superstate::toggle_physics_debugger,
					this)))),
	collision_filter_(
		physics_world_),
	fruit_manager_(
		json::find_member<sge::parse::json::array>(
			context<machine>().config_file(),
			FCPPT_TEXT("fruits")),
		context<machine>().systems().md3_loader(),
		context<machine>().systems().image_loader(),
		context<machine>().systems().renderer(),
		physics_world_,
		context<machine>().camera()),
	fruit_spawner_(
		fruit_manager_,
		context<machine>().config_file(),
		context<machine>().camera(),
		context<machine>().timer_callback()),
	game_logic_(
		context<machine>().timer_callback(),
		context<machine>().config_file(),
		fruit_manager_,
		context<machine>().font_cache(),
		context<machine>().overlay_node(),
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager()),
	cut_connection_(
		fruit_manager_.cut_callback(
			std::tr1::bind(
				&superstate::fruit_was_cut,
				this,
				std::tr1::placeholders::_1)))
{
	// scene
	context<machine>().scene_node().insert_before(
		fruit_manager_,
		context<machine>().point_sprites());
	context<machine>().scene_node().insert_dont_care(
		fruit_spawner_);
	context<machine>().scene_node().insert_dont_care(
		game_logic_);
	context<machine>().scene_node().insert_dont_care(
		physics_world_node_);
	// overlay
	context<machine>().overlay_node().insert_dont_care(
		physics_debugger_node_);
}

fruitcut::physics::world &
fruitcut::app::states::ingame::superstate::physics_world()
{
	return physics_world_;
}

fruitcut::physics::world const &
fruitcut::app::states::ingame::superstate::physics_world() const
{
	return physics_world_;
}

fruitcut::app::fruit::manager &
fruitcut::app::states::ingame::superstate::fruit_manager()
{
	return fruit_manager_;
}

fruitcut::app::fruit::manager const &
fruitcut::app::states::ingame::superstate::fruit_manager() const
{
	return fruit_manager_;
}

fruitcut::app::fruit::spawner &
fruitcut::app::states::ingame::superstate::fruit_spawner()
{
	return fruit_spawner_;
}

fruitcut::app::fruit::spawner const &
fruitcut::app::states::ingame::superstate::fruit_spawner() const
{
	return fruit_spawner_;
}

fruitcut::physics::debugger &
fruitcut::app::states::ingame::superstate::physics_debugger()
{
	return physics_debugger_;
}

fruitcut::app::game_logic &
fruitcut::app::states::ingame::superstate::game_logic()
{
	return game_logic_;
}

fruitcut::app::game_logic const &
fruitcut::app::states::ingame::superstate::game_logic() const
{
	return game_logic_;
}

fruitcut::app::states::ingame::superstate::~superstate()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::superstate::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
}

void
fruitcut::app::states::ingame::superstate::fruit_was_cut(
	fruit::cut_context const &)
{
	/*
	score_ = 
		static_cast<fruitcut::app::score>(
			static_cast<sge::renderer::scalar>(
				score_) + 1000 * _area);
	score_font_.text(
		fcppt::lexical_cast<sge::font::text::string>(
			score_));
	*/
	context<machine>().sound_controller().play(
		FCPPT_TEXT("fruit-was-cut"));
}
