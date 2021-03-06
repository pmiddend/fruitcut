#ifndef FRUITAPP_LIGHT_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED
#define FRUITAPP_LIGHT_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED

#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>


namespace fruitapp
{
namespace light
{
// One of the few copyable classes
class directional_light_source
{
public:
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

	sge::renderer::matrix4 const
	transformation() const;

	~directional_light_source();
private:
	sge::renderer::vector3 position_;
	sge::renderer::matrix4 transformation_;
};
}
}

#endif
