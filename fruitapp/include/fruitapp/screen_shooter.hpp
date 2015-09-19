#ifndef FRUITAPP_SCREEN_SHOOTER_HPP_INCLUDED
#define FRUITAPP_SCREEN_SHOOTER_HPP_INCLUDED

#include <fruitapp/quick_log_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
class screen_shooter
{
FCPPT_NONCOPYABLE(
	screen_shooter);
public:
	screen_shooter(
		sge::input::keyboard::device &,
		sge::renderer::device::core &,
		sge::image2d::system &,
		fruitapp::quick_log &);

	~screen_shooter();
private:
	sge::renderer::device::core &renderer_;
	sge::image2d::system &image_loader_;
	fcppt::signal::auto_connection callback_connection_;

	void
	callback(
		fruitapp::quick_log &);
};
}

#endif
