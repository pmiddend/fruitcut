#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/highscore.hpp>
#include <fruitapp/highscore/providers_from_json.hpp>
#include <fruitapp/highscore/provider/connection_base.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/states/menu/highscore.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::states::menu::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care)))),
	providers_(),
	highscore_(),
	main_menu_button_connection_(
		highscore_->register_back_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_button_connection_(
		highscore_->register_quit_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>(),
				awl::main::exit_success()))),
	connection_(),
	message_connection_(),
	error_connection_(),
	list_connection_()
{
	fruitapp::highscore::providers_from_json(
		context<fruitapp::machine>().systems().charconv_system(),
		context<fruitapp::machine>().config_file(),
		providers_);

	highscore_ =
		context<fruitapp::machine>().gui_system().create_highscore(
			providers_);
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

	highscore_->clear_log();

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
	highscore_->process_list(
		entries);
}

void
fruitapp::states::menu::highscore::text_received(
	fcppt::string const &s)
{
	highscore_->append_log(
		s);
}
