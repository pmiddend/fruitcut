#ifndef FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED
#define FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/shared_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
#include <string>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace CEGUI
{
class Combobox;
class Window;
class ListboxTextItem;
class EventArgs;
}

namespace fruitapp
{
namespace gui
{
namespace ce
{
class combobox
{
FCPPT_NONCOPYABLE(
	combobox);
public:
	typedef
	fcppt::function::object<void()>
	choose_callback;

	explicit
	combobox(
		CEGUI::Window &);

	void
	add(
		std::string const &,
		choose_callback const &);

	 ~combobox();
private:
	typedef
	std::vector<choose_callback>
	callback_sequence;

	CEGUI::Combobox &impl_;
	callback_sequence callbacks_;
	CEGUI::Event::ScopedConnection selection_changed_connection_;

	bool
	selection_changed(
		CEGUI::EventArgs const &);
};
}
}
}

#endif
