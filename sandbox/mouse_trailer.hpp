#ifndef FRUITCUT_SANDBOX_MOUSE_TRAILER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_MOUSE_TRAILER_HPP_INCLUDED

#include "../sprite/system.hpp"
#include "../sprite/object.hpp"
#include "particle.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/input/mouse/device_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <vector>

namespace fruitcut
{
namespace sandbox
{
class mouse_trailer
{
public:
	explicit
	mouse_trailer(
		sge::renderer::device_ptr,
		sge::image::multi_loader &,
		sge::input::mouse::device &);

	void
	update();

	void
	render();
private:
	typedef
	std::vector<particle>
	particle_sequence;

	fruitcut::sprite::system ss_;
	fruitcut::sprite::object cursor_;
	particle_sequence particles_;
	fcppt::signal::scoped_connection mouse_axis_connection_;

	void
	callback(
		sge::input::mouse::axis_event const &);
};
}
}

#endif
