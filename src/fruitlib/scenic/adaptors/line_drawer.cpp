#include "line_drawer.hpp"
#include "../events/render.hpp"
#include <sge/line_drawer/object.hpp>
#include <sge/renderer/scoped_transform.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/matrix_mode.hpp>
#include <sge/renderer/projection/orthogonal.hpp>
#include <sge/renderer/projection/near.hpp>
#include <sge/renderer/projection/far.hpp>
#include <sge/renderer/projection/rect.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>

namespace
{
sge::renderer::projection::rect const
projection_rect_from_viewport(
	sge::renderer::pixel_rect const &viewport)
{
	return 
		sge::renderer::projection::rect(
			sge::renderer::projection::rect::vector(
				static_cast<sge::renderer::scalar>(
					viewport.left()),
				static_cast<sge::renderer::scalar>(
					viewport.bottom())),
			sge::renderer::projection::rect::dim(
				static_cast<sge::renderer::scalar>(
					viewport.size().w()),
				static_cast<sge::renderer::scalar>(
					-viewport.size().h())));
}
}

fruitcut::fruitlib::scenic::adaptors::line_drawer::line_drawer(
	scenic::optional_parent const &_parent,
	sge::line_drawer::object &_object,
	sge::renderer::device * const _renderer)
:
	node_base(
		_parent),
	object_(
		_object),
	renderer_(
		_renderer)
{
}

fruitcut::fruitlib::scenic::adaptors::line_drawer::~line_drawer()
{
}

void
fruitcut::fruitlib::scenic::adaptors::line_drawer::react(
	events::render const &)
{
	if(renderer_)
	{
		sge::renderer::scoped_transform projection_scope(
			*renderer_,
			sge::renderer::matrix_mode::projection,
			sge::renderer::projection::orthogonal(
				::projection_rect_from_viewport(
					renderer_->onscreen_target().viewport().get()),
				sge::renderer::projection::near(
					0),
				sge::renderer::projection::far(
					10)));

		sge::renderer::scoped_transform world_scope(
			*renderer_,
			sge::renderer::matrix_mode::world,
			sge::renderer::matrix4::identity());
		
		object_.render();
	}
	else
		object_.render();
}
