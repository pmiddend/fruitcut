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
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
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

	sge::renderer::texture::planar &
	texture();

	sge::renderer::texture::planar const &
	texture() const;

	~shadow_map();
private:
	sge::renderer::device &renderer_;
	sge::camera::object const &camera_;
	fruit::manager &fruit_manager_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::target_ptr target_;
	sge::shader::object shader_;

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
