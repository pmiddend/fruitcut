#include "combobox.hpp"
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/tr1/functional.hpp>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <iostream>

fruitcut::app::gui::combobox::combobox(
	CEGUI::Window &_impl)
:
	impl_(
		dynamic_cast<CEGUI::Combobox &>(
			_impl)),
	values_(),
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
fruitcut::app::gui::combobox::add(
	std::string const &text,
	choose_callback const &callback)
{
	fcppt::container::ptr::push_back_unique_ptr(
		values_,
		fcppt::make_unique_ptr<CEGUI::ListboxTextItem>(
			CEGUI::String(
				text),
			// index
			0u,
			// user data
			static_cast<void*>(
				0), 
			// disabled
			false, 
			// auto delete (this is the important part)
			false));

	impl_.addItem(
		&(*(--values_.end())));

	callbacks_.push_back(
		callback);
}

fruitcut::app::gui::combobox::~combobox()
{
}

bool
fruitcut::app::gui::combobox::selection_changed(
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