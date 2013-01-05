#include <fruitapp/renderable.hpp>
#include <fruitapp/postprocessing/system.hpp>
#include <fruitapp/postprocessing/subsystems/main.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::renderable::renderable(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	fruitapp::postprocessing::system &_postprocessing_system)
:
	node_base(
		_parent),
	scene_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					0)))),
	postprocessing_(
		_postprocessing_system.create_main_subsystem(
			fruitlib::scenic::optional_parent(
				fruitlib::scenic::parent(
					*this,
					fruitlib::scenic::depth(
						1))),
			std::bind(
				&fruitapp::scene::render_children,
				&scene_,
				std::placeholders::_1))),
	overlay_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					2))),
		_renderer,
		*postprocessing_)
{
}

fruitapp::scene &
fruitapp::renderable::scene()
{
	return scene_;
}

fruitapp::scene const &
fruitapp::renderable::scene() const
{
	return scene_;
}

fruitapp::overlay &
fruitapp::renderable::overlay()
{
	return overlay_;
}

fruitapp::overlay const &
fruitapp::renderable::overlay() const
{
	return overlay_;
}

fruitapp::postprocessing::subsystems::main &
fruitapp::renderable::postprocessing_main()
{
	return
		*postprocessing_;
}

fruitapp::renderable::~renderable()
{
}
