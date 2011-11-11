#include "../../media_path.hpp"
#include <fruitapp/fruit/model_to_mesh.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/fruit/material/from_json.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/model/md3/loader.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/mipmap/all_levels.hpp>
#include <sge/renderer/texture/mipmap/auto_generate.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>


namespace
{
fruitapp::fruit::tag const
json_to_tag(
	sge::parse::json::value const &v)
{
	return
		sge::parse::json::get<fcppt::string>(
			v);
}
}

fruitapp::fruit::prototype const
fruitapp::fruit::prototype_from_json(
	sge::parse::json::value const &v,
	sge::model::md3::loader &model_loader,
	sge::image2d::multi_loader &image_loader,
	sge::renderer::device &renderer)
{
	sge::parse::json::object const &o =
		sge::parse::json::get<sge::parse::json::object>(
			v);

	return
		fruit::prototype(
			fruit::model_to_mesh(
				*model_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							sge::parse::json::find_and_convert_member<fcppt::string>(
								o,
								sge::parse::json::path(
									FCPPT_TEXT("model"))))),
			sge::renderer::texture::create_planar_from_path(
				fruitcut::media_path()
					/ FCPPT_TEXT("textures")
					/ FCPPT_TEXT("fruits")
					/
						sge::parse::json::find_and_convert_member<fcppt::string>(
							o,
							sge::parse::json::path(
								FCPPT_TEXT("texture"))),
				renderer,
				image_loader,
				sge::renderer::texture::mipmap::all_levels(
					sge::renderer::texture::mipmap::auto_generate::yes),
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags_field(
					sge::renderer::resource_flags::readable)),
			material::from_json(
				sge::parse::json::find_and_convert_member<sge::parse::json::object>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("material")))),
			fcppt::algorithm::map<fruit::tag_set>(
				sge::parse::json::find_and_convert_member<sge::parse::json::array>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("tags"))).elements,
				&json_to_tag));
}
