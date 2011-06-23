#include "prototype_from_json.hpp"
#include "model_to_mesh.hpp"
#include "material/from_json.hpp"
#include "../../media_path.hpp"
#include "../../fruitlib/json/find_member.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/map.hpp>

namespace
{
fruitcut::app::fruit::tag const
json_to_tag(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<fcppt::string>(
			v);
}
}

fruitcut::app::fruit::prototype const
fruitcut::app::fruit::prototype_from_json(
	sge::parse::json::value const &v,
	sge::model::loader &model_loader,
	sge::image2d::multi_loader &image_loader,
	sge::renderer::device &renderer)
{
	sge::parse::json::object const &o = 
		sge::parse::json::get<sge::parse::json::object>(
			v);

	return 
		fruit::prototype(
			fruit::model_to_mesh(
				model_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							fruitlib::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("model")))),
			sge::renderer::texture::create_planar_from_path(
				fruitcut::media_path()
					/ FCPPT_TEXT("textures")
					/ FCPPT_TEXT("fruits")
					/
						fruitlib::json::find_member<fcppt::string>(
							o,
							FCPPT_TEXT("texture")),
				renderer,
				image_loader,
				sge::renderer::texture::filter::trilinear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::readable),
			material::from_json(
				fruitlib::json::find_member<sge::parse::json::object>(
					o,
					FCPPT_TEXT("material"))),
			fcppt::algorithm::map<fruit::tag_sequence>(
				fruitlib::json::find_member<sge::parse::json::array>(
					o,
					FCPPT_TEXT("tags")).elements,
				&json_to_tag));
}
