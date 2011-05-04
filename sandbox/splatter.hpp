#ifndef FRUITCUT_SANDBOX_SPLATTER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SPLATTER_HPP_INCLUDED

#include "splat_collector.hpp"
#include "splat_particle.hpp"
#include "../particle/sprite/system.hpp"
#include "../particle/sprite/object.hpp"
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/input/mouse/device_fwd.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/time/funit.hpp>
#include <fcppt/random/uniform.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <utility>
#include <vector>

namespace fruitcut
{
namespace sandbox
{
class splatter
{
public:
	explicit
	splatter(
		sge::renderer::device &,
		sge::image2d::multi_loader &,
		sge::input::mouse::device &);

	void
	update();

	void
	render();
private:
	typedef
	std::vector<sge::renderer::texture::planar_ptr>
	texture_vector;

	// Intentionally a list, I want to iterate and throw away dead
	// particles
	typedef
	boost::ptr_list<splat_particle>
	particle_list;

	splat_collector splat_collector_;
	particle::sprite::system ss_;
	particle::sprite::object cursor_;
	particle::sprite::object canvas_;
	texture_vector textures_;
	fcppt::random::uniform<texture_vector::size_type> texture_rng_;
	fcppt::random::uniform<double> color_rng_;
	fcppt::random::uniform<sge::renderer::scalar> rotation_rng_;
	std::pair<sge::renderer::scalar,sge::renderer::scalar> speed_range_;
	fcppt::random::uniform<sge::renderer::scalar> speed_rng_;
	fcppt::random::uniform<sge::time::funit> lifetime_rng_;
	fcppt::signal::scoped_connection move_connection_,click_connection_;
	particle_list particles_;
	particle::sprite::object::vector first_position_;

	void
	click_callback(
		sge::input::mouse::button_event const &);

	void
	move_callback(
		sge::input::mouse::axis_event const &);

	texture_vector const
	load_textures(
		sge::renderer::device &,
		sge::image2d::multi_loader &);
};
}
}

#endif
