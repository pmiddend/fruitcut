#include <fruitapp/media_path.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/postprocessing/subsystems/main.hpp>
#include <fruitapp/states/loading.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/font/align_h.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/font/lit.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/start.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/insert_to_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <iterator>
#include <fcppt/config/external_end.hpp>


fruitapp::states::loading::loading(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care)))),
	fruits_(
		sge::parse::json::parse_file_exn(
			fruitapp::media_path() / FCPPT_TEXT("fruits.json"))),
	fruit_array_(
		fruits_.array().elements),
	current_fruit_(
		fruit_array_.begin()),
	font_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		fruitlib::font::object_parameters(
			context<fruitapp::machine>().font_manager(),
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("score"))),
			sge::font::string(
				SGE_FONT_LIT("0")),
			sge::font::rect::null(),
			fruitlib::font::align_h::center,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			sge::image::color::any::object(
				fruitlib::json::parse_rgba8_color(
					sge::parse::json::find_and_convert_member<sge::parse::json::value const>(
						context<fruitapp::machine>().config_file(),
						sge::parse::json::path(
							FCPPT_TEXT("loading"))
							/ FCPPT_TEXT("font-color")))),
			fruitlib::font::scale(
				1.f))),
	viewport_change_connection_(
		context<fruitapp::machine>().viewport_manager().change_callback(
			fruitapp::viewport::change_callback{
				std::bind(
					&loading::viewport_change,
					this,
					std::placeholders::_1
				)
			},
			fruitapp::viewport::trigger_early(
				true)))
{
	context<fruitapp::machine>().postprocessing_main().loading_progress(
		static_cast<sge::renderer::scalar>(
			0));
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

	context<fruitapp::machine>().fruit_prototypes().push_back(
		fruitapp::fruit::prototype_from_json(
			*current_fruit_++,
			context<fruitapp::machine>().md3_loader(),
			context<fruitapp::machine>().texture_manager()));

	font_node_.object().text(
		SGE_FONT_LIT("Loaded ")+
		fcppt::insert_to_string<sge::font::string>(
			std::distance(
				fruit_array_.begin(),
				current_fruit_))+
		SGE_FONT_LIT(" of ")+
		fcppt::insert_to_string<sge::font::string>(
			fruit_array_.size())+
		SGE_FONT_LIT(" fruits"));

	context<fruitapp::machine>().postprocessing_main().loading_progress(
		static_cast<sge::renderer::scalar>(
			std::distance(
				fruit_array_.begin(),
				current_fruit_)) /
		static_cast<sge::renderer::scalar>(
			fruit_array_.size()));
}

void
fruitapp::states::loading::viewport_change(
	sge::renderer::target::viewport const &_viewport)
{
	sge::font::rect::dim const &viewport_dim =
		fcppt::math::dim::structure_cast<sge::font::rect::dim, fcppt::cast::size_fun>(
				_viewport.get().size());

	font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::rect::vector::null(),
			viewport_dim));
}


