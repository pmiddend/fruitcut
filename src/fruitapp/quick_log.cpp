#include <fruitapp/quick_log.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/font/color_format.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/json/path.hpp>
#include <sge/parse/json/json.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/from_fcppt_string.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/image/color/convert.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/algorithm/string/join.hpp>
#include <iostream>

fruitapp::quick_log::quick_log(
	fruitlib::scenic::optional_parent const &_parent,
	sge::parse::json::object const &_config_file,
	fruitlib::font::cache &_font_cache,
	sge::renderer::device const &_renderer,
	fruitlib::audio::sound_controller &_sound_controller)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	sound_controller_(
		_sound_controller),
	font_node_(
		fruitlib::scenic::parent(
			*this,
			fruitlib::scenic::depth(
				0)),
		fruitlib::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("quick-log")),
			_font_cache.drawer(
				FCPPT_TEXT("quick-log")),
			sge::font::text::string(),
			// Can't define a box yet, we might have no viewport
			sge::font::rect::null(),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		sge::image::color::convert<fruitlib::font::color_format>(
			fruitlib::json::parse_rgba8_color(
				fruitlib::json::find_and_convert_member<sge::parse::json::value>(
					_config_file,
					fruitlib::json::path(
						FCPPT_TEXT("quick-log")) / FCPPT_TEXT("font-color")))),
		fruitlib::font::scale(
			1)),
	fractional_size_(
		fruitlib::json::find_and_convert_member<fractional_dimension>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("quick-log")) / FCPPT_TEXT("screen-percentage"))),
	message_delete_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			fruitlib::time_format::find_and_convert_duration<sge::timer::clocks::standard::duration>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("quick-log")) / FCPPT_TEXT("message-deletion-time"))).
		active(
			false)),
	messages_()
{
	react(
		fruitlib::scenic::events::viewport_change());
}

void
fruitapp::quick_log::add_message(
	fcppt::string const &new_message)
{
	messages_.push_front(
		sge::font::text::from_fcppt_string(
			new_message));

	if(!message_delete_timer_.active())
		message_delete_timer_.active(
			true);

	sound_controller_.play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("quick_log")));
}

void
fruitapp::quick_log::react(
	fruitlib::scenic::events::update const &e)
{
	if(message_delete_timer_.active() && sge::timer::reset_when_expired(message_delete_timer_))
	{
		messages_.pop_back();
		if(messages_.empty())
			message_delete_timer_.active(
				false);
	}

	font_node_.object().text(
		boost::algorithm::join(
			messages_,
			// It's a template function, better use the "real" container
			// type, so sge::font::text::string()
			sge::font::text::string(
				SGE_FONT_TEXT_LIT("\n"))));

	node_base::forward_to_children(
		e);
}

void
fruitapp::quick_log::react(
	fruitlib::scenic::events::viewport_change const &)
{
	sge::renderer::screen_size const viewport_size = 
		sge::renderer::viewport_size(
			renderer_);

	// No viewport yet? Quit!
	if(!viewport_size.content())
		return;

	font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::rect::vector::null(),
			fcppt::math::dim::structure_cast<sge::font::rect::dim>(
				fcppt::math::dim::structure_cast<fractional_dimension>(
					viewport_size) * 
				fractional_size_)));
}

