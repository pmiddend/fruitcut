#ifndef FRUITAPP_GUI_CREATE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_GUI_CREATE_SYSTEM_HPP_INCLUDED

#include <fruitapp/gui/system_unique_ptr.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/mouse/device_fwd.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>


namespace fruitapp
{
namespace gui
{
fruitapp::gui::system_unique_ptr
create_system(
	fruitlib::scenic::parent const &,
	sge::renderer::device::ffp &,
	sge::image2d::system &,
	sge::viewport::manager &,
	sge::charconv::system &,
	fruitlib::scenic::delta::callback const &,
	sge::input::keyboard::device &,
	sge::input::cursor::object &,
	sge::input::mouse::device &,
	fruitlib::audio::sound_controller &);
}
}

#endif
