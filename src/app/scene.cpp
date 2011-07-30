#include "scene.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/update.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>

fruitcut::app::scene::scene(
	fruitlib::scenic::parent const &_parent,
	sge::systems::instance const &_systems,
	sge::parse::json::object const &_config_file)
:
	node_base(
		_parent),
	active_(
		true),
	postprocessing_(
		_systems.renderer(),
		std::tr1::bind(
			&scene::render_children,
			this),
		fruitlib::json::find_and_convert_member<sge::parse::json::object>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("pp"))))
{
}

void
fruitcut::app::scene::active(
	bool const _active)
{
	active_ = _active;
}

bool
fruitcut::app::scene::active() const
{
	return active_;
}

fruitcut::app::postprocessing &
fruitcut::app::scene::postprocessing()
{
	return postprocessing_;
}

fruitcut::app::postprocessing const &
fruitcut::app::scene::postprocessing() const
{
	return postprocessing_;
}

// Confusing semantics here, sorry. :/
void
fruitcut::app::scene::react(
	fruitlib::scenic::events::render const &)
{
	if (!active_)
		return;

	postprocessing_.render_result();
}

void
fruitcut::app::scene::react(
	fruitlib::scenic::events::update const &e)
{
	if (!active_)
		return;

	node_base::forward_to_children(
		e);

	// This calls intrusive_group::render which renders the children in
	// the according scoped_block
	postprocessing_.update();
}

void
fruitcut::app::scene::render_children()
{
	node_base::forward_to_children(
		fruitlib::scenic::events::render());
}

