#ifndef FRUITCUT_APP_GUI_COMBOBOX_HPP_INCLUDED
#define FRUITCUT_APP_GUI_COMBOBOX_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/shared_connection.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <CEGUIEvent.h>
#include <vector>
#include <string>

namespace CEGUI
{
class Combobox;
class Window;
class ListboxTextItem;
class EventArgs;
}

namespace fruitcut
{
namespace app
{
namespace gui
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
	boost::ptr_vector<CEGUI::ListboxTextItem>
	value_sequence;

	typedef
	std::vector<choose_callback>
	callback_sequence;

	CEGUI::Combobox &impl_;
	value_sequence values_;
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
