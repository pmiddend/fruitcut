#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/model_to_mesh.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/fruit/material/from_json.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/model/md3/loader.hpp>
#include <sge/model/md3/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/mipmap/all_levels.hpp>
#include <sge/renderer/texture/mipmap/auto_generate.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>


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

fruitapp::fruit::prototype_unique_ptr
fruitapp::fruit::prototype_from_json(
	sge::parse::json::value const &v,
	sge::model::md3::loader &model_loader,
	sge::image2d::system &image_loader,
	sge::renderer::device::core &renderer)
{
	sge::parse::json::object const &o =
		sge::parse::json::get<sge::parse::json::object>(
			v);

	return
		fcppt::make_unique_ptr<fruitapp::fruit::prototype>(
			fruitapp::fruit::model_to_mesh(
				*model_loader.load(
					fruitapp::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							sge::parse::json::find_and_convert_member<fcppt::string>(
								o,
								sge::parse::json::path(
									FCPPT_TEXT("model"))))),
			sge::renderer::texture::planar_shared_ptr(
				sge::renderer::texture::create_planar_from_path(
					fruitapp::media_path()
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
					sge::renderer::resource_flags_field(
						sge::renderer::resource_flags::readable))),
			fcppt::cref(
				fruitapp::fruit::material::from_json(
					sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
						o,
						sge::parse::json::path(
							FCPPT_TEXT("material"))))),
			fruitapp::fruit::splatter_color(
				sge::image::color::any::object(
					fruitlib::json::parse_rgba8_color(
						sge::parse::json::find_and_convert_member<sge::parse::json::value const>(
							o,
							sge::parse::json::path(
								FCPPT_TEXT("splatter-color")))))),
			fcppt::cref(
				fcppt::algorithm::map<fruit::tag_set>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array const>(
						o,
						sge::parse::json::path(
							FCPPT_TEXT("tags"))).elements,
					&json_to_tag)));
}
