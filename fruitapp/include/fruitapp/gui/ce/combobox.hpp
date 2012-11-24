#ifndef FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED
#define FRUITAPP_GUI_CE_COMBOBOX_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/strong_typedef.hpp>
#include <fcppt/function/object.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/shared_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
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

	FCPPT_MAKE_STRONG_TYPEDEF(
		bool,
		selected);

	combobox(
		sge::charconv::system &,
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

	sge::charconv::system &charconv_system_;
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
