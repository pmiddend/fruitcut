#include "highscore.hpp"
#include "main.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../highscore/providers_from_json.hpp"
#include "../../highscore/provider/object_base.hpp"
#include "../../highscore/provider/connection_base.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUICombobox.h>
#include <iostream>

fruitcut::app::states::menu::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	fruitlib::scenic::nodes::intrusive(),
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
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>()))),
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
		context<app::machine>().systems().charconv_system(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/List"),
		table_model_)
{
	context<app::machine>().root_node().insert_dont_care(
		*this);

	app::highscore::providers_from_json(
		context<app::machine>().config_file(),
		providers_);

	for(
		app::highscore::provider_sequence::iterator i = 
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

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::highscore)

fruitcut::app::states::menu::highscore::~highscore()
{
}

void
fruitcut::app::states::menu::highscore::switch_provider(
	app::highscore::provider::object_base &new_provider)
{
	std::cout << "Switching the provider, creating a new connection\n";
	connection_ = 
		new_provider.create_connection();
	std::cout << "done\n";

	context<app::machine>().gui_system().window_manager().getWindow(
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

	std::cout << "Retrieving list\n";
	connection_->retrieve_list();
	std::cout << "Done\n";
}

void
fruitcut::app::states::menu::highscore::list_received(
	app::highscore::entry_set const &entries)
{
	std::cout << "list received callback\n";
	table_model_.reset(
		entries);
}

void
fruitcut::app::states::menu::highscore::text_received(
	fcppt::string const &s)
{
	std::cout << "got some text: " << s << "\n";
	CEGUI::Window &w = 
		*context<app::machine>().gui_system().window_manager().getWindow(
			"Highscores/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<app::machine>().systems().charconv_system()));
}

void
fruitcut::app::states::menu::highscore::update()
{
	if(connection_)
		connection_->update();
}

void
fruitcut::app::states::menu::highscore::render()
{
}
