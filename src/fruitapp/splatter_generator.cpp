#if 0
#include <fruitapp/splatter_generator.hpp>
#include <fruitapp/fruit/cut_context.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/fruit/triangle_traits.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/point_sprite/unique_base_ptr.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitlib/uniform_random_range_element.hpp>
#include <fruitlib/json/parse_random_float_distribution.hpp>
#include <fruitlib/json/parse_random_int_distribution.hpp>
#include <fruitlib/math/multiply_matrix4_vector3.hpp>
#include <fruitlib/math/triangle/random_point.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/config/user_config_variable.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <mizuiro/color/channel/alpha.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <limits>
#include <fcppt/config/external_end.hpp>


fruitapp::splatter_generator::splatter_generator(
	sge::parse::json::object const &config_file,
	sge::parse::json::config::user_config_variable<fruit::area::value_type> &_splatter_count_to_area_factor,
	point_sprite::system_node &_point_sprites,
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
			cut_direction_rng::distribution::min(
				0u),
			cut_direction_rng::distribution::max(
				1u))),
	speed_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("speed-range")))),
	distortion_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("speed-distortion-range")))),
	size_rng_(
		random_generator_,
		fruitlib::json::parse_random_float_distribution<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("size-range")))),
	alpha_rng_(
		random_generator_,
		alpha_rng::distribution(
			alpha_rng::distribution::min(
				static_cast<point_sprite::color_format::channel_type>(
					0u)),
			alpha_rng::distribution::max(
				static_cast<point_sprite::color_format::channel_type>(
					std::numeric_limits<point_sprite::color_format::channel_type>::max()/2)))),
	lifetime_millis_rng_(
		random_generator_,
		fruitlib::json::parse_random_int_distribution<boost::chrono::milliseconds::rep>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("lifetime-millis-range")))),
	splatter_count_to_area_factor_(
		_splatter_count_to_area_factor)
{
}

void
fruitapp::splatter_generator::fruit_was_cut(
	fruit::cut_context const &cut_info)
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
			triangle_point_rng_type::distribution::min(
				0.0f),
			triangle_point_rng_type::distribution::sup(
				1.0f)));

	for(
		unsigned
			i =
				0,
			number_of_points =
				static_cast<unsigned>(
					cut_info.area() * splatter_count_to_area_factor_.value() * 200.0f);
		i < number_of_points;
		++i)
	{
		sge::renderer::vector3 const position =
			fruitlib::math::triangle::random_point(
				triangle_rng.value(),
				triangle_point_rng);

		point_sprite::color splatter_color =
			sge::image::color::any::convert<point_sprite::color::format>(
				cut_info.old().prototype().splatter_color());

		splatter_color.set(
			mizuiro::color::channel::alpha(),
			alpha_rng_());

		point_sprite::splatter::linear_velocity::value_type distortion(
			distortion_rng_(),
			distortion_rng_(),
			distortion_rng_());

		point_sprites_.push_back(
			point_sprite::unique_base_ptr(
				fcppt::make_unique_ptr<point_sprite::splatter::object>(
					point_sprite::splatter::parameters(
						point_sprites_.connection(),
						point_sprite::splatter::position(
							cut_info.old().position() +
							fruitlib::math::multiply_matrix4_vector3(
								cut_info.old().world_transform(),
								position)),
						point_sprite::splatter::linear_velocity(
							distortion + ((cut_direction_rng_()
							?
								cut_info.cut_direction()
							:
								(-cut_info.cut_direction())) * speed_rng_())),
						point_sprite::splatter::acceleration(
							acceleration_),
						point_sprite::splatter::size(
							size_rng_()),
						splatter_color,
						point_sprites_.lookup_texture(
							fruitlib::resource_tree::path(
								FCPPT_TEXT("spray"))),
						boost::chrono::duration_cast<fruitapp::ingame_clock::duration>(
							boost::chrono::milliseconds(
								lifetime_millis_rng_())),
						clock_))));
	}
}

fruitapp::splatter_generator::~splatter_generator()
{
}
#endif
