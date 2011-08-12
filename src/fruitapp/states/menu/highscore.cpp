#include <fruitapp/states/menu/highscore.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/highscore/providers_from_json.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/highscore/provider/connection_base.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/depth.hpp>
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUICombobox.h>
#include <iostream>

fruitapp::states::menu::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::parent(
			context<fruitapp::machine>().root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care))),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("new_highscore.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("Highscores")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/MainMenu")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/Quit")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>()))),
	providers_(),
	source_box_(
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/Source")),
	connection_(),
	message_connection_(),
	error_connection_(),
	list_connection_(),
	table_model_(),
	table_view_(
		context<fruitapp::machine>().systems().charconv_system(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/List"),
		table_model_)
{
	fruitapp::highscore::providers_from_json(
		context<fruitapp::machine>().config_file(),
		providers_);

	for(
		fruitapp::highscore::provider_sequence::iterator i =
			providers_.begin();
		i != providers_.end();
		++i)
		source_box_.add(
			fcppt::to_std_string(
				i->identifier()),
			std::tr1::bind(
				&highscore::switch_provider,
				this,
				fcppt::ref(
					*i)));
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::highscore)

fruitapp::states::menu::highscore::~highscore()
{
}

void
fruitapp::states::menu::highscore::react(
	fruitlib::scenic::events::update const &)
{
	if(connection_)
		connection_->update();
}

void
fruitapp::states::menu::highscore::switch_provider(
	fruitapp::highscore::provider::object_base &new_provider)
{
	connection_ =
		new_provider.create_connection();

	context<fruitapp::machine>().gui_system().window_manager().getWindow(
		"Highscores/MessageLog")->setText(
		"");

	message_connection_ =
		connection_->message_received(
			std::tr1::bind(
				&highscore::text_received,
				this,
				std::tr1::placeholders::_1));

	error_connection_ =
		connection_->error_received(
			std::tr1::bind(
				&highscore::text_received,
				this,
				std::tr1::placeholders::_1));

	list_connection_ =
		connection_->list_received(
			std::tr1::bind(
				&highscore::list_received,
				this,
				std::tr1::placeholders::_1));

	connection_->retrieve_list();
}

void
fruitapp::states::menu::highscore::list_received(
	fruitapp::highscore::entry_set const &entries)
{
	table_model_.reset(
		entries);
}

void
fruitapp::states::menu::highscore::text_received(
	fcppt::string const &s)
{
	CEGUI::Window &w =
		*context<fruitapp::machine>().gui_system().window_manager().getWindow(
			"Highscores/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<fruitapp::machine>().systems().charconv_system()));
}
