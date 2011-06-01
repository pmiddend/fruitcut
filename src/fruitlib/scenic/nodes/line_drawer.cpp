#include "line_drawer.hpp"
#include <sge/line_drawer/object.hpp>
#include <sge/renderer/renderer.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/box/box.hpp>

fruitcut::fruitlib::scenic::nodes::line_drawer::line_drawer(
	sge::line_drawer::object &_object,
	sge::renderer::device *_renderer)
:
	object_(
		_object),
	renderer_(
		_renderer)
{
}

fruitcut::fruitlib::scenic::nodes::line_drawer::~line_drawer()
{
}

void
fruitcut::fruitlib::scenic::nodes::line_drawer::render()
{
	if(renderer_)
	{
		sge::renderer::scoped_transform projection_scope(
			*renderer_,
			sge::renderer::matrix_mode::projection,
			fcppt::math::matrix::orthogonal(
				static_cast<sge::renderer::scalar>(
					0),
				static_cast<sge::renderer::scalar>(
					renderer_->onscreen_target().viewport().get().w()),
				static_cast<sge::renderer::scalar>(
					0),
				static_cast<sge::renderer::scalar>(
					renderer_->onscreen_target().viewport().get().h()),
				static_cast<sge::renderer::scalar>(
					0),
				static_cast<sge::renderer::scalar>(
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

void
fruitcut::fruitlib::scenic::nodes::line_drawer::update()
{
}
