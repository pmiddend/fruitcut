#ifndef FRUITCUT_FRUITLIB_SCREEN_SHOOTER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCREEN_SHOOTER_HPP_INCLUDED

#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace fruitlib
{
class screen_shooter
{
FCPPT_NONCOPYABLE(
	screen_shooter);
public:
	explicit
	screen_shooter(
		fcppt::string const &,
		sge::input::keyboard::device &,
		sge::renderer::device &,
		sge::image2d::multi_loader &);

	~screen_shooter();
private:
	fcppt::string const app_name_;
	sge::renderer::device &renderer_;
	sge::image2d::multi_loader &image_loader_;
	fcppt::signal::scoped_connection callback_connection_;

	void
	callback();
};
}
}

#endif
