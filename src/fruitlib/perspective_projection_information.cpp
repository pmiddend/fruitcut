#include <fruitlib/perspective_projection_information.hpp>

fruitlib::perspective_projection_information::perspective_projection_information(
	sge::renderer::projection::fov const &_fov,
	sge::renderer::projection::near const &_near,
	sge::renderer::projection::far const &_far,
	sge::renderer::projection::aspect const &_aspect)
:
	fov_(
		_fov),
	near_(
		_near),
	far_(
		_far),
	aspect_(
		_aspect)
{
}

sge::renderer::projection::fov const &
fruitlib::perspective_projection_information::fov() const
{
	return
		fov_;
}

sge::renderer::projection::near const &
fruitlib::perspective_projection_information::near() const
{
	return
		near_;
}

sge::renderer::projection::far const &
fruitlib::perspective_projection_information::far() const
{
	return
		far_;
}

sge::renderer::projection::aspect const &
fruitlib::perspective_projection_information::aspect() const
{
	return
		aspect_;
}
