#include "renderable.hpp"
#include "../fruitlib/scenic/parent.hpp"
#include "../fruitlib/scenic/depth.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/update.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/json/path.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>

fruitcut::app::renderable::renderable(
	fruitlib::scenic::optional_parent const &_parent,
	sge::systems::instance const &_systems,
	sge::parse::json::object const &_config_file)
:
	node_base(
		_parent),
	scene_(
		fruitlib::scenic::parent(
			*this,
			fruitlib::scenic::depth(
				0))),
	postprocessing_(
		fruitlib::scenic::parent(
			*this,
			fruitlib::scenic::depth(
				1)),
		_systems.renderer(),
		std::tr1::bind(
			&scene::render_children,
			&scene_),
		fruitlib::json::find_and_convert_member<sge::parse::json::object>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("pp")))),
	overlay_(
		fruitlib::scenic::parent(
			*this,
			fruitlib::scenic::depth(
				2)),
		_systems.renderer(),
		postprocessing_)
{
}

fruitcut::app::scene &
fruitcut::app::renderable::scene()
{
	return scene_;
}

fruitcut::app::scene const &
fruitcut::app::renderable::scene() const
{
	return scene_;
}

fruitcut::app::overlay &
fruitcut::app::renderable::overlay()
{
	return overlay_;
}

fruitcut::app::overlay const &
fruitcut::app::renderable::overlay() const
{
	return overlay_;
}

fruitcut::app::postprocessing &
fruitcut::app::renderable::postprocessing()
{
	return postprocessing_;
}

fruitcut::app::postprocessing const &
fruitcut::app::renderable::postprocessing() const
{
	return postprocessing_;
}

fruitcut::app::renderable::~renderable()
{
}
