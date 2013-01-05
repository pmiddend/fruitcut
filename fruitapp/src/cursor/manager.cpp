#include <fruitapp/cursor/instance_sequence.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <sge/input/processor.hpp>
#include <sge/input/cursor/discover_event.hpp>
#include <sge/input/cursor/remove_event.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::cursor::manager::manager(
	sge::input::processor &_input_processor,
	sge::renderer::device::ffp &_renderer,
	sge::renderer::target::base const &_target,
	sge::camera::base const &_camera,
	fruitlib::texture_manager &_texture_manager,
	fruitapp::viewport::manager &_viewport_manager,
	fruitlib::audio::sound_controller &_sound_controller,
	fruitapp::ingame_clock const &_ingame_clock,
	sge::parse::json::object const &_configuration)
:
	renderer_(
		_renderer),
	target_(
		_target),
	camera_(
		_camera),
	texture_manager_(
		_texture_manager),
	viewport_manager_(
		_viewport_manager),
	sound_controller_(
		_sound_controller),
	ingame_clock_(
		_ingame_clock),
	configuration_(
		_configuration),
	discover_connection_(
		_input_processor.cursor_discover_callback(
			std::bind(
				&fruitapp::cursor::manager::discover,
				this,
				std::placeholders::_1))),
	remove_connection_(
		_input_processor.cursor_remove_callback(
			std::bind(
				&fruitapp::cursor::manager::remove,
				this,
				std::placeholders::_1))),
	cursors_(),
	instance_sequences_()
{
}

fruitapp::cursor::instance_sequence_unique_ptr
fruitapp::cursor::manager::create_instances(
	fruitlib::scenic::optional_parent const &_parent)
{
	fruitapp::cursor::instance_sequence_unique_ptr new_instance(
		fcppt::make_unique_ptr<fruitapp::cursor::instance_sequence>(
			*this,
			_parent));

	fruitapp::cursor::instance_sequence * const new_instance_ptr =
		&*new_instance;

	instance_sequences_.push_back(
		new_instance_ptr);

	for(
		cursor_sequence::iterator it =
			cursors_.begin();
		it != cursors_.end();
		++it)
		new_instance->cursor_discover(
			*it);

	return
		std::move(
			new_instance);
}

fruitapp::cursor::manager::~manager()
{
}

void
fruitapp::cursor::manager::discover(
	sge::input::cursor::discover_event const &_discover)
{
	sge::input::cursor::object * const cptr =
		&(_discover.get());

	cursors_.push_back(
		cptr);

	for(
		instance_sequence_sequence::iterator it =
			instance_sequences_.begin();
		it != instance_sequences_.end();
		++it)
		it->cursor_discover(
			_discover.get());
}

void
fruitapp::cursor::manager::remove(
	sge::input::cursor::remove_event const &_remove)
{
	for(
		instance_sequence_sequence::iterator it =
			instance_sequences_.begin();
		it != instance_sequences_.end();
		++it)
		it->cursor_remove(
			_remove.get());

	fcppt::algorithm::ptr_container_erase(
		cursors_,
		&(_remove.get()));
}

void
fruitapp::cursor::manager::instance_sequence_destroyed(
	fruitapp::cursor::instance_sequence &_instance_sequence)
{
	fcppt::algorithm::ptr_container_erase(
		instance_sequences_,
		&_instance_sequence);
}
