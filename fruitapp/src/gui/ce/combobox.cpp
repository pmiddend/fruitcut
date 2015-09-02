#include <fruitapp/gui/ce/combobox.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::combobox::combobox(
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
			std::bind(
				&combobox::selection_changed,
				this,
				std::placeholders::_1)))
{
}

void
fruitapp::gui::ce::combobox::add(
	fcppt::string const &_text,
	choose_callback const &_callback,
	selected const &_selected)
{
	CEGUI::ListboxItem * const new_item =
		new CEGUI::ListboxTextItem(
			sge::cegui::to_cegui_string(
				_text));

	impl_.addItem(
		new_item);

	if(_selected.get())
	{
		impl_.setItemSelectState(
			new_item,
			true);
		impl_.handleUpdatedListItemData();
	}

	callbacks_.push_back(
		_callback);
}

fruitapp::gui::ce::combobox::~combobox()
{
}

bool
fruitapp::gui::ce::combobox::selection_changed(
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
