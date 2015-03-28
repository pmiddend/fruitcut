#ifndef FRUITLIB_PERSPECTIVE_PROJECTION_INFORMATION_HPP_INCLUDED
#define FRUITLIB_PERSPECTIVE_PROJECTION_INFORMATION_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <sge/renderer/projection/aspect.hpp>
#include <sge/renderer/projection/far.hpp>
#include <sge/renderer/projection/fov.hpp>
#include <sge/renderer/projection/near.hpp>


namespace fruitlib
{
class perspective_projection_information
{
public:
	FRUITLIB_DETAIL_SYMBOL
	perspective_projection_information(
		sge::renderer::projection::fov const &,
		sge::renderer::projection::near const &,
		sge::renderer::projection::far const &,
		sge::renderer::projection::aspect const &);

	sge::renderer::projection::fov const &
	fov() const;

	sge::renderer::projection::near const &
	near() const;

	sge::renderer::projection::far const &
	far() const;

	sge::renderer::projection::aspect const &
	aspect() const;
private:
	sge::renderer::projection::fov fov_;
	sge::renderer::projection::near near_;
	sge::renderer::projection::far far_;
	sge::renderer::projection::aspect aspect_;
};
}

#endif

