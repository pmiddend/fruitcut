#ifndef FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED
#define FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/strong_typedef.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
#include <functional>
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
	std::function<void()>
	choose_callback;

	FCPPT_MAKE_STRONG_TYPEDEF(
		bool,
		selected);

	explicit
	combobox(
		CEGUI::Window &);

	void
	add(
		fcppt::string const &,
		choose_callback const &,
		selected const &);

	 ~combobox();
private:
	typedef
	std::vector<choose_callback>
	callback_sequence;

	CEGUI::Combobox &impl_;
	callback_sequence callbacks_;
	CEGUI::Event::ScopedConnection selection_changed_connection_;
};
}
}
}

#endif
