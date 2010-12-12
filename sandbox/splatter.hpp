#ifndef FRUITCUT_SANDBOX_SPLATTER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SPLATTER_HPP_INCLUDED

#include "splat_collector.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/input/mouse/device_fwd.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <fcppt/random/uniform.hpp>
#include <fcppt/signal/scoped_connection.hpp>
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
		sge::renderer::device_ptr,
		sge::image::multi_loader &,
		sge::input::mouse::device &);

	void
	update();

	void
	render();
private:
	void
	click_callback(
		sge::input::mouse::button_event const &);

	void
	move_callback(
		sge::input::mouse::axis_event const &);
private:
	typedef
	std::vector<sge::renderer::texture_ptr>
	texture_vector;

	typedef
	std::vector<sprite::object>
	particle_vector;

	splat_collector splat_collector_;
	sprite::system ss_;
	sprite::object cursor_;
	sprite::object canvas_;
	texture_vector textures_;
	fcppt::random::uniform<texture_vector::size_type> rng_;
	fcppt::random::uniform<double> color_rng_;
	fcppt::signal::scoped_connection move_connection_,click_connection_;
	particle_vector particles_;

	texture_vector const
	load_textures(
		sge::renderer::device_ptr,
		sge::image::multi_loader &);
};
}
}

#endif
