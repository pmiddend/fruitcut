#include "scene.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>

fruitcut::app::scene::scene(
	sge::systems::instance const &_systems,
	sge::parse::json::object const &_config_file)
:
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

// Confusing semantics here, sorry. :/
void
fruitcut::app::scene::render()
{
	if (!active_)
		return;
	postprocessing_.render_result();
}

void
fruitcut::app::scene::update()
{
	if (!active_)
		return;

	intrusive_group::update();

	// This calls intrusive_group::render which renders the children in
	// the according scoped_block
	postprocessing_.update();
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

void
fruitcut::app::scene::render_children()
{
	intrusive_group::render();
}


