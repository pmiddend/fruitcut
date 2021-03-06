#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/model_to_mesh.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/fruit/material/from_json.hpp>
#include <fruitlib/texture_manager.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/model/md3/load_flags_field.hpp>
#include <sge/model/md3/loader.hpp>
#include <sge/model/md3/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/get_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/mipmap/all_levels.hpp>
#include <sge/renderer/texture/mipmap/auto_generate.hpp>
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
		sge::parse::json::get_exn<fcppt::string>(
			v);
}
}

fruitapp::fruit::prototype_unique_ptr
fruitapp::fruit::prototype_from_json(
	sge::parse::json::value const &v,
	sge::model::md3::loader &model_loader,
	fruitlib::texture_manager &_texture_manager)
{
	sge::parse::json::object const &o =
		sge::parse::json::get_exn<sge::parse::json::object>(
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
									FCPPT_TEXT("model"))),
					sge::model::md3::load_flags_field::null()
				)
			),
			sge::renderer::texture::planar_shared_ptr(
				_texture_manager.create_planar_from_path(
					fruitapp::media_path()
						/ FCPPT_TEXT("textures")
						/ FCPPT_TEXT("fruits")
						/
							sge::parse::json::find_and_convert_member<fcppt::string>(
								o,
								sge::parse::json::path(
									FCPPT_TEXT("texture"))),
					sge::renderer::texture::mipmap::all_levels(
						sge::renderer::texture::mipmap::auto_generate::yes),
					sge::renderer::resource_flags_field{
						sge::renderer::resource_flags::readable})),
			fruitapp::fruit::material::from_json(
				sge::parse::json::find_and_convert_member<sge::parse::json::object>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("material")))),
			fruitapp::fruit::splatter_color(
				sge::image::color::any::object(
					fruitlib::json::parse_rgba8_color(
						sge::parse::json::find_and_convert_member<sge::parse::json::value>(
							o,
							sge::parse::json::path(
								FCPPT_TEXT("splatter-color")))))),
			fcppt::algorithm::map<fruit::tag_set>(
				sge::parse::json::find_and_convert_member<sge::parse::json::array>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("tags"))).elements,
				&json_to_tag));
}
