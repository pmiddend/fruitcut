#include <fruitapp/states/loading.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <fruitlib/font/color_format.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitapp/postprocessing.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/text/flags.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/string.hpp>
#include <sge/font/dim.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/convert.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <iterator>

fruitapp::states::loading::loading(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::parent(
			context<fruitapp::machine>().root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care))),
	fruit_array_(
		fruitlib::json::find_and_convert_member<sge::parse::json::array>(
			context<machine>().config_file(),
			fruitlib::json::path(
				FCPPT_TEXT("fruits"))).elements),
	current_fruit_(
		fruit_array_.begin()),
	font_node_(
		fruitlib::scenic::parent(
			context<fruitapp::machine>().overlay_node(),
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
		react(
			fruitlib::scenic::events::viewport_change());
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::superstate,
	loading)

fruitapp::states::loading::~loading()
{
}

void
fruitapp::states::loading::react(
	fruitlib::scenic::events::update const &)
{
	if(current_fruit_ == fruit_array_.end())
	{
		FRUITAPP_EVENTS_POST_TRANSITION(
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
fruitapp::states::loading::react(
	fruitlib::scenic::events::viewport_change const &)
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


