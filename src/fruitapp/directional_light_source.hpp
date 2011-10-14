#ifndef FRUITAPP_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED
#define FRUITAPP_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED

#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace fruitapp
{
// One of the few copyable classes
class directional_light_source
{
public:
	explicit
	directional_light_source();

	// We store the position separate because the diffuse/specular
	// shading doesn't need the direction
	sge::renderer::vector3 const &
	position() const;

	void
	position(
		sge::renderer::vector3 const &);

	// NOT the transformation here but the translation included
	sge::renderer::matrix4 const
	model_view() const;

	void
	transformation(
		sge::renderer::matrix4 const &);

	~directional_light_source();
private:
	sge::renderer::vector3 position_;
	sge::renderer::matrix4 transformation_;
};
}

#endif
