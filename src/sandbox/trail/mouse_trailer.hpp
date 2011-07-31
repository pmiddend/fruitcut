#ifndef FRUITCUT_SANDBOX_MOUSE_TRAILER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_MOUSE_TRAILER_HPP_INCLUDED

#include <fruitlib/particle/sprite/system.hpp>
#include <fruitlib/particle/sprite/object.hpp>
#include <sandbox/trail/sword_particle.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/time/timer.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/input/mouse/device_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <list>

namespace fruitcut
{
namespace sandbox
{
class mouse_trailer
{
public:
	explicit
	mouse_trailer(
		sge::renderer::device &,
		sge::image2d::multi_loader &,
		sge::input::mouse::device &);

	void
	update();

	void
	render();

	~mouse_trailer();
private:
	typedef
	std::list<sword_particle>
	particle_sequence;

	fruitcut::fruitlib::particle::sprite::system ss_;
	fruitcut::fruitlib::particle::sprite::object cursor_;
	particle_sequence particles_;
	fcppt::signal::scoped_connection mouse_axis_connection_;
	sge::texture::part_ptr particle_texture_;
	sge::time::timer update_timer_;
	sge::time::timer output_timer_;
	sge::renderer::vector2 old_position_;

	void
	callback(
		sge::input::mouse::axis_event const &);
};
}
}

#endif
