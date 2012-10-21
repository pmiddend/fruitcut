#ifndef FRUITAPP_SHADOW_MAP_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_HPP_INCLUDED

#include <fruitapp/shadow_map_texture.hpp>
#include <fruitapp/shadow_mvp.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_scoped_ptr.hpp>
#include <sge/renderer/target/offscreen_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
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
	public fruitlib::scenic::node<shadow_map>
{
FCPPT_NONCOPYABLE(
	shadow_map);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	shadow_map(
		fruitlib::scenic::optional_parent const &,
		sge::parse::json::object const &,
		sge::renderer::device::ffp &,
		sge::renderer::matrix4 const &);

	fruitapp::shadow_map_texture const
	texture()
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::shadow_mvp const
	mvp() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	~shadow_map();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::renderer::device::ffp &renderer_;
	sge::renderer::state::core::depth_stencil::object_scoped_ptr const depth_stencil_state_;
	sge::renderer::texture::planar_scoped_ptr const texture_;
	sge::renderer::target::offscreen_scoped_ptr const target_;
	sge::renderer::matrix4 mvp_;
};
}

#endif
