#include "loading.hpp"
#include "menu/main.hpp"
#include "../fruit/prototype_from_json.hpp"
#include "../events/define_transition_reaction.hpp"
#include "../depths/root.hpp"
#include "../depths/overlay.hpp"
#include "../events/post_transition.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/json/parse_rgba8_color.hpp"
#include "../../fruitlib/font/object_parameters.hpp"
#include "../../fruitlib/font/color_format.hpp"
#include "../../fruitlib/scenic/parent.hpp"
#include "../../fruitlib/scenic/events/render.hpp"
#include "../../fruitlib/scenic/events/update.hpp"
#include "../../fruitlib/scenic/depth.hpp"
#include "../../fruitlib/font/scale.hpp"
#include "../../fruitlib/font/cache.hpp"
#include "../postprocessing.hpp"
#include <sge/renderer/viewport_size.hpp>
#include <sge/font/font.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <iterator>

fruitcut::app::states::loading::loading(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::parent(
			context<app::machine>().root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care))),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&loading::viewport_change,
				this))),
	fruit_array_(
		fruitlib::json::find_and_convert_member<sge::parse::json::array>(
			context<machine>().config_file(),
			fruitlib::json::path(
				FCPPT_TEXT("fruits"))).elements),
	current_fruit_(
		fruit_array_.begin()),
	font_node_(
		fruitlib::scenic::parent(
			context<app::machine>().overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		fruitlib::font::object_parameters(
			context<machine>().font_cache().metrics(
				FCPPT_TEXT("score")),
			context<machine>().font_cache().drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		sge::image::color::convert<fruitlib::font::color_format>(
			fruitlib::json::parse_rgba8_color(
				fruitlib::json::find_and_convert_member<sge::parse::json::value>(
					context<machine>().config_file(),
					fruitlib::json::path(
						FCPPT_TEXT("loading"))
						/ FCPPT_TEXT("font-color")))),
		fruitlib::font::scale(
			1))
{
	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			0));

	// We already hae a viewport? Ok, then go
	if(sge::renderer::viewport_size(context<machine>().systems().renderer()).content())
		viewport_change();
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::superstate,
	loading)

fruitcut::app::states::loading::~loading()
{
}

void
fruitcut::app::states::loading::react(
	fruitlib::scenic::events::update const &)
{
	if(current_fruit_ == fruit_array_.end())
	{
		FRUITCUT_APP_EVENTS_POST_TRANSITION(
			menu::superstate);
		return;
	}

	context<machine>().fruit_prototypes().push_back(
		fruit::prototype_from_json(
			*current_fruit_++,
			context<machine>().md3_loader(),
			context<machine>().systems().image_loader(),
			context<machine>().systems().renderer()));

	font_node_.object().text(
		SGE_FONT_TEXT_LIT("Loaded ")+
		fcppt::lexical_cast<sge::font::text::string>(
			std::distance(
				fruit_array_.begin(),
				current_fruit_))+
		SGE_FONT_TEXT_LIT(" of ")+
		fcppt::lexical_cast<sge::font::text::string>(
			fruit_array_.size())+
		SGE_FONT_TEXT_LIT(" fruits"));

	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			std::distance(
				fruit_array_.begin(),
				current_fruit_)) / 
			static_cast<sge::renderer::scalar>(
				fruit_array_.size()));
}

void
fruitcut::app::states::loading::viewport_change()
{
	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			sge::renderer::viewport_size(
				context<machine>().systems().renderer()));

	font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			viewport_dim));
}


