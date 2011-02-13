#ifndef FRUITCUT_APP_FRUIT_PROTOTYPE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PROTOTYPE_HPP_INCLUDED

#include "../mesh.hpp"
#include <sge/renderer/texture_ptr.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class prototype
{
public:
	explicit
	prototype(
		fruitcut::app::mesh const &,
		sge::renderer::texture_ptr);

	fruitcut::app::mesh const &
	mesh() const;

	sge::renderer::texture_ptr const 
	texture() const;
private:
	fruitcut::app::mesh mesh_;
	sge::renderer::texture_ptr texture_;
};
}
}
}

#endif
