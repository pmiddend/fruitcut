#ifndef FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED

#include "prototype.hpp"
#include "object_sequence.hpp"
#include "object_fwd.hpp"
#include "../plane.hpp"
#include "../../physics/world_fwd.hpp"
#include <sge/parse/json/array_fwd.hpp>
#include <sge/model/loader_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/shader/object.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <fcppt/noncopyable.hpp>
#include <vector>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::parse::json::array const &v,
		sge::model::loader &model_loader,
		sge::image2d::multi_loader &image_loader,
		sge::renderer::device_ptr renderer,
		physics::world &);

	// I don't think the manager needs the whole camera, so pass the mvp here
	void
	render(
		sge::renderer::matrix4 const &);

	void
	update();

	void
	cut_fruit(
		object const &,
		plane const &);

	object_sequence const &
	fruits() const;

	~manager();
private:
	typedef
	std::vector<prototype>
	prototype_sequence;

	typedef
	std::vector<object const*>
	old_fruit_list;

	typedef
	boost::ptr_list<object>
	new_fruit_list;

	sge::renderer::device_ptr renderer_;
	physics::world &physics_world_;
	prototype_sequence prototypes_;
	object_sequence fruits_; 
	sge::shader::object fruit_shader_;
	old_fruit_list old_fruits_;
	new_fruit_list new_fruits_;
};
}
}
}

#endif
