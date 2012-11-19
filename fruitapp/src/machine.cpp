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

#define FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(return_type,name) return_type fruitapp::machine::name() { return impl_->name(); }
#define FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(return_type,name) return_type fruitapp::machine::name() const { return impl_->name(); }

FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(sge::parse::json::object const &,config_file)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::systems const &,systems)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(sge::shader::optional_context_ref const,shader_context)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::texture_manager &,texture_manager)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::graphics_settings::object &,graphics_settings)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(sge::model::md3::loader &,md3_loader)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::config_variables &,config_variables)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::postprocessing::system &,postprocessing_system)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::postprocessing::subsystems::main &,postprocessing_main)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::ingame_clock const &,ingame_clock)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::audio::sound_controller &,sound_controller)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitlib::audio::sound_controller const &,sound_controller)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::audio::music_controller &,music_controller)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitlib::audio::music_controller const &,music_controller)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::light::manager &,light_manager)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::shadow_map::optional_object_ref const,shadow_map)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(sge::camera::first_person::object &,camera)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(sge::camera::first_person::object const &,camera)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::font::manager &,font_manager)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::cursor::manager &,cursor_manager)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::gui::system &,gui_system)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::random_generator &,random_generator)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::highscore::score const,last_game_score)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::scene &,scene_node)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::scene const &,scene_node)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::overlay &,overlay_node)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::overlay const &,overlay_node)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::point_sprite::system_node &,point_sprites)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::point_sprite::system_node const &,point_sprites)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::ingame_clock::float_type,time_factor)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(sge::renderer::texture::emulate_srgb::type,emulate_srgb)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitlib::scenic::base &,root_node)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitlib::scenic::delta::callback const,ingame_clock_callback)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitlib::scenic::delta::callback const,standard_clock_callback)
FRUITAPP_MACHINE_CONST_MEMBER_FUNCTION(fruitapp::fruit::prototype_sequence const &,fruit_prototypes)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::fruit::prototype_sequence &,fruit_prototypes)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::quick_log &,quick_log)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::viewport::manager &,viewport_manager)
FRUITAPP_MACHINE_NONCONST_MEMBER_FUNCTION(fruitapp::projection_manager::object &,projection_manager)

awl::main::exit_code const
fruitapp::machine::run()
{
	while(impl_->run_once())
	{
		for(
			queued_event_list::const_iterator current_event = queued_events_.begin();
			current_event != queued_events_.end();
			++current_event)
		{
			base::process_event(
				**current_event);
		}
		queued_events_.clear();
	}

	return impl_->exit_code();
}

void
fruitapp::machine::last_game_score(
	fruitapp::highscore::score const &_last_game_score)
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


void
fruitapp::machine::time_factor(
	fruitapp::ingame_clock::float_type const _time_factor)
{
	impl_->time_factor(
		_time_factor);
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

