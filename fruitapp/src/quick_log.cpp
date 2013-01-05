#include <fruitapp/quick_log.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/font/from_fcppt_string.hpp>
#include <sge/font/lit.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/algorithm/string/join.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::quick_log::quick_log(
	fruitlib::scenic::optional_parent const &_parent,
	sge::parse::json::object const &_config_file,
	fruitlib::font::manager &_font_manager,
	fruitapp::viewport::manager &_viewport_manager,
	fruitlib::audio::sound_controller &_sound_controller)
:
	node_base(
		_parent),
	sound_controller_(
		_sound_controller),
	font_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					0))),
		fruitlib::font::object_parameters(
			_font_manager,
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("quick-log"))),
			// Can't define a box yet, we might have no viewport
			sge::font::string(),
			sge::font::rect::null(),
			sge::font::align_h::left,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			sge::image::color::any::object(
				fruitlib::json::parse_rgba8_color(
					sge::parse::json::find_and_convert_member<sge::parse::json::value const>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("font-color"))))),
			fruitlib::font::scale(
				1.f))),
	fractional_size_(
		sge::parse::json::find_and_convert_member<fractional_dimension>(
			_config_file,
			sge::parse::json::path(
				FCPPT_TEXT("screen-percentage")))),
	message_delete_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			fruitlib::time_format::find_and_convert_duration<sge::timer::clocks::standard::duration>(
				_config_file,
				sge::parse::json::path(
					FCPPT_TEXT("message-deletion-time")))).
		active(
			false)),
	messages_(),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::bind(
				&quick_log::viewport_change,
				this,
				std::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true)))
{
}

void
fruitapp::quick_log::add_message(
	fcppt::string const &new_message)
{
	messages_.push_front(
		sge::font::from_fcppt_string(
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
			// type, so sge::font::string()
			sge::font::string(
				SGE_FONT_LIT("\n"))));

	node_base::forward_to_children(
		e);
}

void
fruitapp::quick_log::viewport_change(
	sge::renderer::target::viewport const &_viewport)
{
	sge::renderer::pixel_rect::dim const viewport_size(
		_viewport.get().size());

	font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::rect::vector::null(),
			fcppt::math::dim::structure_cast<sge::font::rect::dim>(
				fcppt::math::dim::structure_cast<fractional_dimension>(
					viewport_size) *
				fractional_size_)));
}

