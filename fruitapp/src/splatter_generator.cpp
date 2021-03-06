#include <fruitapp/splatter_generator.hpp>
#include <fruitapp/fruit/cut_context.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/fruit/triangle_traits.hpp>
#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitlib/uniform_random_range_element.hpp>
#include <fruitlib/json/parse_random_float_distribution.hpp>
#include <fruitlib/json/parse_random_int_distribution.hpp>
#include <fruitlib/math/triangle/random_point.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/texture/part_fwd.hpp>
#include <mizuiro/color/channel/alpha.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/reference_impl.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/math/matrix/transform_direction.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/optional/maybe_void.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <limits>
#include <fcppt/config/external_end.hpp>


fruitapp::splatter_generator::splatter_generator(
	sge::parse::json::object const &config_file,
	fruitapp::point_sprite::system_node &_point_sprites,
	fruitlib::random_generator &_random_generator,
	point_sprite::splatter::acceleration const &_acceleration,
	fruitapp::ingame_clock const &_clock)
:
	point_sprites_(
		_point_sprites),
	random_generator_(
		_random_generator),
	acceleration_(
		_acceleration.get()),
	clock_(
		_clock),
	cut_direction_rng_(
		random_generator_,
		cut_direction_rng::distribution(
			cut_direction_rng::distribution::param_type::min(
				0u),
			cut_direction_rng::distribution::param_type::max(
				1u))),
	speed_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(
					FCPPT_TEXT("speed-range"))))),
	distortion_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(
					FCPPT_TEXT("speed-distortion-range"))))),
	size_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("size-range"))))),
	alpha_rng_(
		random_generator_,
		alpha_rng::distribution(
			alpha_rng::distribution::param_type::min(
				static_cast<fruitapp::point_sprite::color_format::channel_type>(
					0u)),
			alpha_rng::distribution::param_type::max(
				static_cast<fruitapp::point_sprite::color_format::channel_type>(
					std::numeric_limits<fruitapp::point_sprite::color_format::channel_type>::max()/2)))),
	lifetime_millis_rng_(
		random_generator_,
		fruitlib::json::parse_random_int_distribution<std::chrono::milliseconds::rep>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(
					FCPPT_TEXT("lifetime-millis-range"))))),
	cut_area_multiplier_(
		sge::parse::json::find_and_convert_member<fruitapp::fruit::area::value_type>(
			config_file,
			sge::parse::json::path(
				FCPPT_TEXT("cut-area-multiplier"))))
{
}

void
fruitapp::splatter_generator::fruit_was_cut(
	fruitapp::fruit::cut_context const &cut_info)
{
	if(cut_info.cross_section().triangles().empty())
		return;

	typedef
	fruitlib::uniform_random_range_element
	<
		fruitapp::fruit::mesh::triangle_sequence const,
		fruitlib::random_generator
	>
	triangle_rng_type;

	triangle_rng_type triangle_rng(
		random_generator_,
		cut_info.cross_section().triangles());

	typedef
	fruitlib::uniform_real_random<sge::renderer::scalar>::type
	triangle_point_rng_type;

	triangle_point_rng_type triangle_point_rng(
		random_generator_,
		triangle_point_rng_type::distribution(
			triangle_point_rng_type::distribution::param_type::min(
				0.0f),
			triangle_point_rng_type::distribution::param_type::sup(
				1.0f)));

	for(
		unsigned
			i =
				0,
			number_of_points =
				static_cast<unsigned>(
					(cut_info.area() * cut_area_multiplier_).get());
		i < number_of_points;
		++i)
	{
		sge::renderer::vector3 const position(
			fruitlib::math::triangle::random_point(
				triangle_rng.value(),
				triangle_point_rng));

		fruitapp::point_sprite::color splatter_color(
			sge::image::color::any::convert<fruitapp::point_sprite::color::format>(
				cut_info.old().prototype().splatter_color()));

		splatter_color.set(
			mizuiro::color::channel::alpha(),
			alpha_rng_());

		fruitapp::point_sprite::splatter::linear_velocity::value_type distortion(
			distortion_rng_(),
			distortion_rng_(),
			distortion_rng_());

		fcppt::optional::maybe_void(
			point_sprites_.lookup_texture(
				fruitlib::resource_tree::path(
					FCPPT_TEXT("spray")
				)
			),
			[
				&distortion,
				&cut_info,
				position,
				splatter_color,
				this
			](
				fcppt::reference<
					sge::texture::part const
				> const _texture
			)
			{
				point_sprites_.push_back(
					fcppt::unique_ptr_to_base<
						fruitapp::point_sprite::base
					>(
						fcppt::make_unique_ptr<
							fruitapp::point_sprite::splatter::object
						>(
							fruitapp::point_sprite::splatter::parameters(
								point_sprites_.projection_manager(),
								point_sprites_.connection(),
								fruitapp::point_sprite::splatter::position(
									cut_info.old().position() +
									fcppt::math::matrix::transform_direction(
										cut_info.old().world_transform(),
										position)),
								fruitapp::point_sprite::splatter::linear_velocity(
									distortion + ((cut_direction_rng_()
									?
										cut_info.cut_geometry().cut_direction().get()
									:
									(-cut_info.cut_geometry().cut_direction().get())) * speed_rng_())),
								fruitapp::point_sprite::splatter::acceleration(
									acceleration_),
								fruitapp::point_sprite::splatter::size(
									size_rng_()),
								splatter_color,
								_texture,
								std::chrono::duration_cast<fruitapp::ingame_clock::duration>(
									std::chrono::milliseconds(
										lifetime_millis_rng_())),
								clock_
							)
						)
					)
				);
			}
		);
	}
}

fruitapp::splatter_generator::~splatter_generator()
{
}
