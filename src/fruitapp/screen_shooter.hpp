#ifndef FRUITAPP_SCREEN_SHOOTER_HPP_INCLUDED
#define FRUITAPP_SCREEN_SHOOTER_HPP_INCLUDED

#include <fruitapp/quick_log_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
class screen_shooter
{
FCPPT_NONCOPYABLE(
	screen_shooter);
public:
	explicit
	screen_shooter(
		sge::input::keyboard::device &,
		sge::renderer::device &,
		sge::image2d::multi_loader &,
		fruitapp::quick_log &);

	~screen_shooter();
private:
	sge::renderer::device &renderer_;
	sge::image2d::multi_loader &image_loader_;
	fcppt::signal::scoped_connection callback_connection_;

	void
	callback(
		fruitapp::quick_log &);
};
}

#endif
