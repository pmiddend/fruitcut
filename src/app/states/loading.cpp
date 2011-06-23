#include "loading.hpp"
#include "menu.hpp"
#include "ingame/running.hpp"
#include "../fruit/prototype_from_json.hpp"
#include "../events/generic_transition.hpp"
#include "../../fruitlib/json/find_member.hpp"
#include "../../fruitlib/json/parse_color.hpp"
#include "../../fruitlib/font/object_parameters.hpp"
#include "../../fruitlib/font/cache.hpp"
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/font/font.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/colors.hpp>
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
	fruitlib::scenic::nodes::intrusive(),
	scoped_render_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f)),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&loading::viewport_change,
				this))),
	fruit_array_(
		fruitlib::json::find_member<sge::parse::json::array>(
			context<machine>().config_file(),
			FCPPT_TEXT("fruits")).elements),
	current_fruit_(
		fruit_array_.begin()),
	font_node_(
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
		fruitlib::json::parse_color<sge::image::color::rgba8>(
			fruitlib::json::find_member<sge::parse::json::value>(
				context<machine>().config_file(),
				FCPPT_TEXT("loading/font-color"))),
		static_cast<fruitlib::scenic::scale>(
			1))
{
	// We already hae a viewport? Ok, then go
	if(sge::renderer::viewport_size(context<machine>().systems().renderer()).content())
		viewport_change();

	context<machine>().root_node().insert_dont_care(
		*this);

	context<machine>().overlay_node().insert_dont_care(
		font_node_);
}

void
fruitcut::app::states::loading::update()
{
	if(current_fruit_ == fruit_array_.end())
	{
		context<machine>().post_event(
			events::generic_transition<states::menu>());
		return;
	}

	context<machine>().fruit_prototypes().push_back(
		fruit::prototype_from_json(
			*current_fruit_++,
			context<machine>().systems().md3_loader(),
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
}

void
fruitcut::app::states::loading::render()
{
}

fruitcut::app::states::loading::~loading()
{
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
