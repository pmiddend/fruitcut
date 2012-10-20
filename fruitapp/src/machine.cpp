#include <fruitapp/exception.hpp>
#include <fruitapp/machine.hpp>
#include <fruitapp/machine_impl.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/cegui/syringe_fwd.hpp>
#include <sge/cegui/system.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_code.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/config/external_begin.hpp>
#include <typeinfo>
#include <fcppt/config/external_end.hpp>


fruitapp::machine::machine(
	int argc,
	char *argv[])
:
	impl_(
		fcppt::make_unique_ptr<fruitapp::machine_impl>(
			argc,
			argv)),
	queued_events_()
{
}

sge::parse::json::object const &
fruitapp::machine::config_file() const
{
	return impl_->config_file();
}

fruitapp::systems const &
fruitapp::machine::systems() const
{
	return impl_->systems();
}

sge::shader::context &
fruitapp::machine::shader_context() const
{
	return impl_->shader_context();
}

sge::model::md3::loader &
fruitapp::machine::md3_loader()
{
	return impl_->md3_loader();
}

fruitapp::config_variables &
fruitapp::machine::config_variables()
{
	return impl_->config_variables();
}

fruitapp::config_variables const &
fruitapp::machine::config_variables() const
{
	return impl_->config_variables();
}

fruitapp::postprocessing &
fruitapp::machine::postprocessing()
{
	return impl_->postprocessing();
}

awl::main::exit_code const
fruitapp::machine::run()
{
	while(impl_->run_once())
	{
		for(
			queued_event_list::const_iterator current_event = queued_events_.begin();
			current_event != queued_events_.end();
			++current_event)
			base::process_event(
				**current_event);
		queued_events_.clear();
	}

	return impl_->exit_code();
}

fruitapp::ingame_clock const &
fruitapp::machine::ingame_clock() const
{
	return impl_->ingame_clock();
}

fruitlib::audio::sound_controller &
fruitapp::machine::sound_controller()
{
	return impl_->sound_controller();
}

fruitlib::audio::sound_controller const &
fruitapp::machine::sound_controller() const
{
	return impl_->sound_controller();
}

fruitlib::audio::music_controller &
fruitapp::machine::music_controller()
{
	return impl_->music_controller();
}

fruitlib::audio::music_controller const &
fruitapp::machine::music_controller() const
{
	return impl_->music_controller();
}

fruitapp::background &
fruitapp::machine::background()
{
	return impl_->background();
}

fruitapp::background const &
fruitapp::machine::background() const
{
	return impl_->background();
}

fruitapp::directional_light_source const &
fruitapp::machine::main_light_source()
{
	return impl_->main_light_source();
}

fruitapp::shadow_map &
fruitapp::machine::shadow_map()
{
	return impl_->shadow_map();
}

fruitapp::shadow_map const &
fruitapp::machine::shadow_map() const
{
	return impl_->shadow_map();
}

sge::camera::first_person::object &
fruitapp::machine::camera()
{
	return impl_->camera();
}

sge::camera::first_person::object const &
fruitapp::machine::camera() const
{
	return impl_->camera();
}

fruitlib::font::cache &
fruitapp::machine::font_cache()
{
	return impl_->font_cache();
}

fruitlib::font::cache const &
fruitapp::machine::font_cache() const
{
	return impl_->font_cache();
}

fruitapp::gui::system &
fruitapp::machine::gui_system()
{
	return
		impl_->gui_system();
}

fruitlib::random_generator &
fruitapp::machine::random_generator()
{
	return impl_->random_generator();
}

fruitapp::highscore::score::value_type
fruitapp::machine::last_game_score() const
{
	return impl_->last_game_score();
}

void
fruitapp::machine::last_game_score(
	highscore::score::value_type const &_last_game_score)
{
	impl_->last_game_score(
		_last_game_score);
}

void
fruitapp::machine::quit(
	awl::main::exit_code const _exit_code)
{
	impl_->quit(
		_exit_code);
}

fruitapp::scene &
fruitapp::machine::scene_node()
{
	return impl_->scene_node();
}

fruitapp::scene const &
fruitapp::machine::scene_node() const
{
	return impl_->scene_node();
}

fruitapp::overlay &
fruitapp::machine::overlay_node()
{
	return impl_->overlay_node();
}

fruitapp::overlay const &
fruitapp::machine::overlay_node() const
{
	return impl_->overlay_node();
}

fruitapp::point_sprite::system_node &
fruitapp::machine::point_sprites()
{
	return impl_->point_sprites();
}

fruitapp::point_sprite::system_node const &
fruitapp::machine::point_sprites() const
{
	return impl_->point_sprites();
}

fruitapp::ingame_clock::float_type
fruitapp::machine::time_factor() const
{
	return impl_->time_factor();
}

void
fruitapp::machine::time_factor(
	fruitapp::ingame_clock::float_type const _time_factor)
{
	impl_->time_factor(
		_time_factor);
}

fruitlib::scenic::base &
fruitapp::machine::root_node()
{
	return impl_->root_node();
}

fruitlib::scenic::delta::callback const
fruitapp::machine::ingame_clock_callback() const
{
	return impl_->ingame_clock_callback();
}

fruitlib::scenic::delta::callback const
fruitapp::machine::standard_clock_callback() const
{
	return impl_->standard_clock_callback();
}

fruitapp::machine::~machine()
{
}

void
fruitapp::machine::unconsumed_event(
	boost::statechart::event_base const &e)
{
	throw
		fruitapp::exception(
			FCPPT_TEXT("Got an unconsumed event of type \"")+
			fcppt::type_name(
				typeid(e))+
			FCPPT_TEXT("\""));
}

void
fruitapp::machine::post_event(
	boost::statechart::event_base const &e)
{
	queued_events_.push_back(
		e.intrusive_from_this());
}

fruitapp::fruit::prototype_sequence const &
fruitapp::machine::fruit_prototypes() const
{
	return impl_->fruit_prototypes();
}

fruitapp::fruit::prototype_sequence &
fruitapp::machine::fruit_prototypes()
{
	return impl_->fruit_prototypes();
}

fruitapp::quick_log &
fruitapp::machine::quick_log()
{
	return impl_->quick_log();
}

fruitapp::viewport::manager &
fruitapp::machine::viewport_manager()
{
	return impl_->viewport_manager();
}

fruitapp::projection_manager::object &
fruitapp::machine::projection_manager()
{
	return impl_->projection_manager();
}
