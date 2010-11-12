#include "perspective.hpp"

fruitcut::graphics::camera::projection::perspective::perspective(
	scalar const _aspect,
	scalar const _fov,
	scalar const _near,
	scalar const _far)
:
	aspect_(
		_aspect),
	fov_(
		_fov),
	near_(
		_near),
	far_(
		_far)
{
}

fruitcut::scalar 
fruitcut::graphics::camera::projection::perspective::aspect() const
{
	return aspect_;
}

fruitcut::scalar 
fruitcut::graphics::camera::projection::perspective::fov() const
{
	return fov_;
}

fruitcut::scalar 
fruitcut::graphics::camera::projection::perspective::near() const
{
	return near_;
}

fruitcut::scalar 
fruitcut::graphics::camera::projection::perspective::far() const
{
	return far_;
}
