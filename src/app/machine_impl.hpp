#ifndef FRUITCUT_APP_MACHINE_IMPL_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_IMPL_HPP_INCLUDED

#include "highscore/score.hpp"
#include "background.hpp"
#include "directional_light_source.hpp"
#include "config_variables.hpp"
#include "quick_log.hpp"
#include "../fruitlib/audio/music_controller.hpp"
#include "../fruitlib/audio/sound_controller.hpp"
#include "../fruitlib/font/cache.hpp"
#include "../fruitlib/log/scoped_sequence_ptr.hpp"
#include "../fruitlib/random_generator.hpp"
#include "../fruitlib/scenic/adaptors/camera.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include "../fruitlib/scenic/events/render_fwd.hpp"
#include "../fruitlib/scenic/node.hpp"
#include "screen_shooter.hpp"
#include "point_sprite/system_node.hpp"
#include "renderable.hpp"
#include "shadow_map.hpp"
#include "fruit/prototype_sequence.hpp"
#include <sge/camera/object.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/system.hpp>
#include <sge/model/md3/loader_ptr.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/time.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/chrono/chrono.hpp>

namespace fruitcut
{
namespace app
{
class machine_impl
:
	public fruitlib::scenic::node<machine_impl>
{
FCPPT_NONCOPYABLE(
	machine_impl);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	machine_impl(
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
	run_once();

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

	fruitlib::random_generator &
	random_generator();

	highscore::score::value_type
	last_game_score() const;

	void
	last_game_score(
		highscore::score::value_type const &);

	fruitlib::scenic::base &
	root_node();

	fruitlib::scenic::base const &
	root_node() const;

	scene &
	scene_node();

	scene const &
	scene_node() const;

	overlay &
	overlay_node();

	overlay const &
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

	fruit::prototype_sequence const &
	fruit_prototypes() const;

	fruit::prototype_sequence &
	fruit_prototypes();

	app::quick_log &
	quick_log();

	~machine_impl();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitlib::random_generator random_generator_;
	sge::parse::json::object user_config_file_;
	sge::parse::json::object const config_file_;
	app::config_variables config_variables_;
	sge::systems::instance const systems_;
	sge::model::md3::loader_ptr md3_loader_;
	app::renderable renderable_;
	fruitlib::log::scoped_sequence_ptr activated_loggers_;
	fruitlib::font::cache font_cache_;
	sge::time::timer second_timer_;
	sge::time::point current_time_,transformed_time_;
	sge::time::funit time_factor_;
	fruitlib::audio::sound_controller sound_controller_;
	fcppt::signal::scoped_connection effects_volume_change_connection_;
	fruitlib::audio::music_controller music_controller_;
	fcppt::signal::scoped_connection music_volume_change_connection_;
	app::quick_log quick_log_;
	sge::camera::object camera_;
	fruitlib::scenic::adaptors::camera camera_node_;
	fcppt::signal::scoped_connection toggle_camera_connection_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	app::directional_light_source main_light_source_;
	fruitcut::app::shadow_map shadow_map_;
	fruitcut::app::background background_;
	fcppt::chrono::milliseconds::rep desired_fps_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	highscore::score::value_type last_game_score_;
	point_sprite::system_node point_sprites_;
	app::screen_shooter screen_shooter_;
	fruit::prototype_sequence fruit_prototypes_;
	sge::renderer::state::scoped base_render_state_;

	void
	toggle_camera();

	void
	viewport_change();

	void 
	manage_rendering();
};
}
}

#endif
