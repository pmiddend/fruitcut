#ifndef FRUITAPP_BACKGROUND_FFP_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_FFP_HPP_INCLUDED

#include <fruitapp/background/base.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/state/core/sampler/const_object_ref_map.hpp>
#include <sge/renderer/state/core/sampler/object_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace background
{
class ffp
:
	public fruitapp::background::base
{
FCPPT_NONCOPYABLE(
	ffp);
public:
	ffp(
		fruitlib::texture_manager &,
		sge::renderer::device::ffp &,
		sge::parse::json::object const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &);

	void
	render(
		sge::renderer::context::core &);

	~ffp();
private:
	sge::renderer::device::ffp &renderer_;
	sge::camera::base const &camera_;
	sge::renderer::texture::planar_scoped_ptr const texture_;
	sge::renderer::state::core::sampler::object_scoped_ptr const background_sampler_;
	sge::renderer::state::core::sampler::const_object_ref_map const samplers_;
};
}
}

#endif
