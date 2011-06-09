#ifndef FRUITCUT_APP_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED
#define FRUITCUT_APP_DIRECTIONAL_LIGHT_SOURCE_HPP_INCLUDED

#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>

namespace fruitcut
{
namespace app
{
class directional_light_source
{
public:
	explicit
	directional_light_source();

	sge::renderer::vector3 const &
	position() const;

	void	
	position(
		sge::renderer::vector3 const &);

	sge::renderer::matrix4 const 
	modelview() const;

	void
	transformation(
		sge::renderer::matrix4 const &);

	~directional_light_source();
private:
	sge::renderer::vector3 position_;
	sge::renderer::matrix4 transformation_;	
};
}
}

#endif
