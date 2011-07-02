#include "quick_log.hpp"
#include "../fruitlib/font/cache.hpp"
#include "../fruitlib/font/object_parameters.hpp"
#include "../fruitlib/time_format/string_to_duration.hpp"
#include "../fruitlib/font/object.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/json/parse_rgba8_color.hpp"
#include "../fruitlib/json/path.hpp"
#include "../fruitlib/scenic/scale.hpp"
#include "../fruitlib/scenic/color.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/time/time.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/from_fcppt_string.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/screen_size.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/algorithm/string/join.hpp>

fruitcut::app::quick_log::quick_log(
	sge::parse::json::object const &_config_file,
	fruitlib::font::cache &_font_cache,
	sge::viewport::manager &_viewport_manager,
	sge::renderer::device const &_renderer)
:
	font_node_(
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
		fruitlib::scenic::color(
			fruitlib::json::parse_rgba8_color(
				fruitlib::json::find_and_convert_member<sge::parse::json::value>(
					_config_file,
					fruitlib::json::path(
						FCPPT_TEXT("quick-log")) / FCPPT_TEXT("font-color")))),
		static_cast<fruitlib::scenic::scale>(
			1)),
	fractional_size_(
		fruitlib::json::find_and_convert_member<fractional_dimension>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("quick-log")) / FCPPT_TEXT("screen-percentage"))),
	viewport_change_connection_(
		_viewport_manager.manage_callback(
			std::tr1::bind(
				&quick_log::viewport_change,
				this,
				fcppt::cref(
					_renderer)))),
	message_delete_timer_(
		*fruitlib::time_format::string_to_duration<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("quick-log")) / FCPPT_TEXT("message-deletion-time"))),
		sge::time::activation_state::inactive),
	messages_()
{
	viewport_change(
		_renderer);

	intrusive_group::insert_dont_care(
		font_node_);
}

void
fruitcut::app::quick_log::add_message(
	fcppt::string const &new_message)
{
	messages_.push_front(
		sge::font::text::from_fcppt_string(
			new_message));

	if(!message_delete_timer_.active())
		message_delete_timer_.activate();
}

void
fruitcut::app::quick_log::viewport_change(
	sge::renderer::device const &_renderer)
{
	sge::renderer::screen_size const viewport_size = 
		sge::renderer::viewport_size(
			_renderer);

	// No viewport yet? Quit!
	if(!viewport_size.content())
		return;

	font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::rect::vector(),
			fcppt::math::dim::structure_cast<sge::font::rect::dim>(
				fcppt::math::dim::structure_cast<fractional_dimension>(
					viewport_size) * 
				fractional_size_)));
}

void
fruitcut::app::quick_log::update()
{
	if(message_delete_timer_.active() && message_delete_timer_.update_b())
	{
		messages_.pop_back();
		if(messages_.empty())
			message_delete_timer_.deactivate();
	}

	font_node_.object().text(
		boost::algorithm::join(
			messages_,
			// It's a template function, better use the "real" container
			// type, so sge::font::text::string()
			sge::font::text::string(
				SGE_FONT_TEXT_LIT("\n"))));

	intrusive_group::update();
}
