#include <fruitapp/gui/combobox.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::combobox::combobox(
	CEGUI::Window &_impl)
:
	impl_(
		dynamic_cast<CEGUI::Combobox &>(
			_impl)),
	callbacks_(),
	selection_changed_connection_(
		impl_.subscribeEvent(
			// CEGUI::Combobox::EventListSelectionChanged is wrong, this is
			// called when the mouse hovers over an item while selecting
			CEGUI::Combobox::EventListSelectionAccepted,
			std::tr1::bind(
				&combobox::selection_changed,
				this,
				std::tr1::placeholders::_1)))
{
}

void
fruitapp::gui::combobox::add(
	std::string const &text,
	choose_callback const &callback)
{
	impl_.addItem(
		new CEGUI::ListboxTextItem(
			CEGUI::String(
				text)));

	callbacks_.push_back(
		callback);
}

fruitapp::gui::combobox::~combobox()
{
}

bool
fruitapp::gui::combobox::selection_changed(
	CEGUI::EventArgs const &)
{
	CEGUI::ListboxItem const * const selected =
		impl_.getSelectedItem();

	if(selected)
		callbacks_[
			static_cast<callback_sequence::size_type>(
				impl_.getItemIndex(
					selected))]();
	return
		true;
}
