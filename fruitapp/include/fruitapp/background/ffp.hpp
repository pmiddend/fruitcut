#ifndef FRUITAPP_BACKGROUND_FFP_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_FFP_HPP_INCLUDED

#include <fruitapp/background/base.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/state/core/sampler/const_object_ref_map.hpp>
#include <sge/renderer/state/core/sampler/object_unique_ptr.hpp>
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
		sge::renderer::device::ffp &,
		fruitlib::texture_manager &,
		fruitapp::background::repetitions const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &);

	void
	render(
		sge::renderer::context::core &
	)
	override;

	~ffp()
	override;
private:
	sge::renderer::device::ffp &renderer_;
	sge::camera::base const &camera_;
	sge::renderer::state::core::sampler::object_unique_ptr const background_sampler_;
	sge::renderer::state::core::sampler::const_object_ref_map const samplers_;
};
}
}

#endif
