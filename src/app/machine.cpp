#include "machine.hpp"
#include "machine_impl.hpp"
#include "exception.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/type_name.hpp>
#include <typeinfo>

fruitcut::app::machine::machine(
	int argc,
	char *argv[])
:
	impl_(
		fcppt::make_unique_ptr<app::machine_impl>(
			argc,
			argv)),
	queued_events_(),
	running_()
{
}

sge::parse::json::object const &
fruitcut::app::machine::config_file() const
{
	return impl_->config_file();
}

sge::systems::instance const &
fruitcut::app::machine::systems() const
{
	return impl_->systems();
}

sge::model::md3::loader &
fruitcut::app::machine::md3_loader()
{
	return impl_->md3_loader();
}

fruitcut::app::config_variables &
fruitcut::app::machine::config_variables()
{
	return impl_->config_variables();
}

fruitcut::app::config_variables const &
fruitcut::app::machine::config_variables() const
{
	return impl_->config_variables();
}

fruitcut::app::postprocessing &
fruitcut::app::machine::postprocessing()
{
	return impl_->postprocessing();
}

void
fruitcut::app::machine::run()
{
	running_ = true;

	while(running_)
	{
		impl_->run_once();

		for(
			queued_event_list::const_iterator current_event = queued_events_.begin(); 
			current_event != queued_events_.end();
			++current_event)
			base::process_event(
				**current_event);
		queued_events_.clear();
	}
}

sge::time::callback const 
fruitcut::app::machine::timer_callback() const
{
	return impl_->timer_callback();
}

fruitcut::fruitlib::audio::sound_controller &
fruitcut::app::machine::sound_controller()
{
	return impl_->sound_controller();
}

fruitcut::fruitlib::audio::sound_controller const &
fruitcut::app::machine::sound_controller() const
{
	return impl_->sound_controller();
}

fruitcut::fruitlib::audio::music_controller &
fruitcut::app::machine::music_controller()
{
	return impl_->music_controller();
}

fruitcut::fruitlib::audio::music_controller const &
fruitcut::app::machine::music_controller() const
{
	return impl_->music_controller();
}

fruitcut::app::background &
fruitcut::app::machine::background()
{
	return impl_->background();
}

fruitcut::app::background const &
fruitcut::app::machine::background() const
{
	return impl_->background();
}

fruitcut::app::directional_light_source const &
fruitcut::app::machine::main_light_source()
{
	return impl_->main_light_source();
}

fruitcut::app::shadow_map &
fruitcut::app::machine::shadow_map()
{
	return impl_->shadow_map();
}

fruitcut::app::shadow_map const &
fruitcut::app::machine::shadow_map() const
{
	return impl_->shadow_map();
}

sge::camera::object &
fruitcut::app::machine::camera()
{
	return impl_->camera();
}

sge::camera::object const &
fruitcut::app::machine::camera() const
{
	return impl_->camera();
}

fruitcut::fruitlib::font::cache &
fruitcut::app::machine::font_cache()
{
	return impl_->font_cache();
}

fruitcut::fruitlib::font::cache const &
fruitcut::app::machine::font_cache() const
{
	return impl_->font_cache();
}

sge::cegui::system &
fruitcut::app::machine::gui_system()
{
	return impl_->gui_system();
}

sge::cegui::system const &
fruitcut::app::machine::gui_system() const
{
	return impl_->gui_system();
}

sge::cegui::syringe &
fruitcut::app::machine::gui_syringe()
{
	return impl_->gui_syringe();
}

sge::cegui::syringe const &
fruitcut::app::machine::gui_syringe() const
{
	return impl_->gui_syringe();
}

fruitcut::fruitlib::rng_creator &
fruitcut::app::machine::rng_creator()
{
	return impl_->rng_creator();
}

fruitcut::app::highscore::score::value_type
fruitcut::app::machine::last_game_score() const
{
	return impl_->last_game_score();
}

void
fruitcut::app::machine::last_game_score(
	highscore::score::value_type const &_last_game_score)
{
	impl_->last_game_score(
		_last_game_score);
}

void
fruitcut::app::machine::quit()
{
	running_ = false;
}

fruitcut::app::scene &
fruitcut::app::machine::scene_node()
{
	return impl_->scene_node();
}

fruitcut::app::scene const &
fruitcut::app::machine::scene_node() const
{
	return impl_->scene_node();
}

fruitcut::app::overlay &
fruitcut::app::machine::overlay_node()
{
	return impl_->overlay_node();
}

fruitcut::app::overlay const &
fruitcut::app::machine::overlay_node() const
{
	return impl_->overlay_node();
}

fruitcut::app::point_sprite::system_node &
fruitcut::app::machine::point_sprites() 
{
	return impl_->point_sprites();
}

fruitcut::app::point_sprite::system_node const &
fruitcut::app::machine::point_sprites() const 
{
	return impl_->point_sprites();
}

sge::time::funit
fruitcut::app::machine::time_factor() const
{
	return impl_->time_factor();
}

void
fruitcut::app::machine::time_factor(
	sge::time::funit const _time_factor)
{
	impl_->time_factor(
		_time_factor);
}

fruitcut::fruitlib::scenic::nodes::intrusive_group &
fruitcut::app::machine::root_node()
{
	return *impl_;
}

fruitcut::app::machine::~machine()
{
}

void
fruitcut::app::machine::unconsumed_event(
	boost::statechart::event_base const &e)
{
	throw 
		app::exception(
			FCPPT_TEXT("Got an unconsumed event of type \"")+
			fcppt::type_name(
				typeid(e))+
			FCPPT_TEXT("\""));
}

void
fruitcut::app::machine::post_event(
	boost::statechart::event_base const &e)
{
	queued_events_.push_back(
		e.intrusive_from_this());
}

fruitcut::app::fruit::prototype_sequence const &
fruitcut::app::machine::fruit_prototypes() const
{
	return impl_->fruit_prototypes();
}

fruitcut::app::fruit::prototype_sequence &
fruitcut::app::machine::fruit_prototypes()
{
	return impl_->fruit_prototypes();
}

fruitcut::app::quick_log &
fruitcut::app::machine::quick_log()
{
	return impl_->quick_log();
}
