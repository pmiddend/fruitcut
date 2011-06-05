#ifndef FRUITCUT_APP_SHADOW_MAP_HPP_INCLUDED
#define FRUITCUT_APP_SHADOW_MAP_HPP_INCLUDED

#include "fruit/manager.hpp"
#include "../fruitlib/scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/texture/planar_fwd.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <sge/renderer/matrix4.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
/**
	A better way than this:

	The shadow map class should be 
	
	- a container for the shadow map texture
	- a container for the mvp
	- a container for all the scene graph nodes which should be rendered to the shadow map

	So instead of hard-coding the fruit manager here, the fruit manager
	should have its own scene graph node below this one.
 */
class shadow_map
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	shadow_map);
public:
	explicit
	shadow_map(
		sge::parse::json::object const &,
		sge::renderer::device &,
		sge::camera::object const &,
		fruit::manager &);

	sge::renderer::texture::planar_ptr const
	texture();

	sge::renderer::matrix4 const &
	mvp() const;

	~shadow_map();
private:
	sge::renderer::device &renderer_;
	sge::camera::object const &camera_;
	fruit::manager &fruit_manager_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::target_ptr target_;
	sge::shader::object shader_;
	sge::renderer::matrix4 mvp_;

	// override
	void
	render();

	// override
	void
	update();
};
}
}

#endif
