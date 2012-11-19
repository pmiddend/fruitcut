#ifndef FRUITAPP_GUI_SYSTEM_HPP_INCLUDED
#define FRUITAPP_GUI_SYSTEM_HPP_INCLUDED

#include <fruitapp/graphics_settings/object_fwd.hpp>
#include <fruitapp/gui/initial_effects_volume.hpp>
#include <fruitapp/gui/initial_music_volume.hpp>
#include <fruitapp/gui/dialogs/highscore_unique_ptr.hpp>
#include <fruitapp/gui/dialogs/ingame_menu_unique_ptr.hpp>
#include <fruitapp/gui/dialogs/main_menu_unique_ptr.hpp>
#include <fruitapp/gui/dialogs/name_chooser_unique_ptr.hpp>
#include <fruitapp/gui/dialogs/ranking_unique_ptr.hpp>
#include <fruitapp/gui/dialogs/settings_unique_ptr.hpp>
#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/provider_sequence.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace gui
{
class system
{
FCPPT_NONCOPYABLE(
	system);
public:
	virtual fruitapp::gui::dialogs::highscore_unique_ptr
	create_highscore(
		fruitapp::highscore::provider_sequence &) = 0;

	virtual fruitapp::gui::dialogs::ranking_unique_ptr
	create_ranking(
		fruitapp::highscore::provider_sequence &) = 0;

	virtual fruitapp::gui::dialogs::main_menu_unique_ptr
	create_main_menu() = 0;

	virtual fruitapp::gui::dialogs::ingame_menu_unique_ptr
	create_ingame_menu() = 0;

	virtual fruitapp::gui::dialogs::name_chooser_unique_ptr
	create_name_chooser(
		fruitapp::highscore::score const &) = 0;

	virtual fruitapp::gui::dialogs::settings_unique_ptr
	create_settings(
		fruitapp::gui::initial_effects_volume const &,
		fruitapp::gui::initial_music_volume const &,
		fruitapp::graphics_settings::object &) = 0;

	virtual
	~system() = 0;
protected:
	system();
};
}
}

#endif
