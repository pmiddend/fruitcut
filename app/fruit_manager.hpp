#ifndef FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED

#include "../physics/world_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/model/loader_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class fruit_manager
{
FCPPT_NONCOPYABLE(
	fruit_manager);
public:
	explicit
	fruit_manager(
		sge::parse::json::object const &,
		sge::model::loader &,
		sge::image2d::multi_loader &,
		sge::renderer::device &,
		physics::world &);
private:
	typedef
	std::vector<fruit const*>
	old_fruit_list;

	typedef
	boost::ptr_list<fruit>
	new_fruit_list;

	typedef
	std::vector<fruit_prototype>
	prototype_sequence;

	typedef
	boost::ptr_list<fruit>
	fruit_sequence;

	prototype_sequence prototypes_;
	fruit_sequence fruits_; 
	sge::shader::object fruit_shader_;
	old_fruit_list old_fruits_;
	new_fruit_list new_fruits_;
};
}
}

#endif
