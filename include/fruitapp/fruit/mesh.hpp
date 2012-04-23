#ifndef FRUITAPP_FRUIT_MESH_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_HPP_INCLUDED

#include <fruitapp/fruit/triangle.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
class mesh
{
FCPPT_NONCOPYABLE(
	mesh);
public:
	typedef
	std::vector<triangle>
	triangle_sequence;

	explicit
	mesh(
		triangle_sequence const &);

	triangle_sequence const &
	triangles() const
	FCPPT_PP_CONST;

	triangle_sequence &
	triangles()
	FCPPT_PP_CONST;

	~mesh();
private:
	triangle_sequence triangles_;
};
}
}

#endif
