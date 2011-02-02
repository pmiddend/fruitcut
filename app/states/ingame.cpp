#include "ingame.hpp"
#include "running.hpp"
#include "../model_to_mesh.hpp"
#include "../../input/state.hpp"
#include "../../json/find_member.hpp"
#include "../../media_path.hpp"
#include "../events/toggle_pause.hpp"
#include <sge/model/loader.hpp>
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/get.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/string.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/statechart/event_base.hpp>

namespace
{
fruitcut::app::fruit_prototype const
parse_fruit(
	sge::parse::json::value const &v,
	sge::model::loader &model_loader,
	sge::image2d::multi_loader &image_loader,
	sge::renderer::device &renderer)
{
	sge::parse::json::object const &o = 
		sge::parse::json::get<sge::parse::json::object>(
			v);

	return 
		fruitcut::app::fruit_prototype(
			fruitcut::app::model_to_mesh(
				model_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("model")))),
			renderer.create_texture(
				image_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("textures")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("texture")))->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none));
}
}

fruitcut::app::states::ingame::ingame(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				boost::bind(
					// Fucking missing overload resolution
					static_cast<void (ingame::*)(boost::statechart::event_base const &)>(
						&ingame::post_event),
					this,
					events::toggle_pause())))),
	prototypes_(
		fcppt::algorithm::map<prototype_sequence>(
			json::find_member<sge::parse::json::array>(
				context<machine>().config_file(),
				FCPPT_TEXT("fruits")).elements,
			boost::bind(
				&parse_fruit,
				_1,
				boost::ref(
					*context<machine>().systems().md3_loader()),
				boost::ref(
					context<machine>().systems().image_loader()),
				boost::ref(
					*context<machine>().systems().renderer()))))
{
}

fruitcut::app::states::ingame::~ingame()
{
}
