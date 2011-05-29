#include "prototype_from_json.hpp"
#include "model_to_mesh.hpp"
#include "../../media_path.hpp"
#include "../../json/find_member.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/model/loader.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

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
							json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("model")))),
			sge::renderer::texture::create_planar_from_view(
				renderer,
				image_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("textures")
						/ FCPPT_TEXT("fruits")
						/
							json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("texture")))->view(),
				sge::renderer::texture::filter::trilinear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::readable));
}