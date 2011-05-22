#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/rect.hpp>
#include <sge/image2d/view/sub.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/from_std_string.hpp>
#include <boost/type_traits/promote.hpp>
#include <mizuiro/color/homogenous.hpp>
#include <mizuiro/color/set.hpp>
#include <mizuiro/color/get.hpp>
#include <mizuiro/color/for_each_channel.hpp>
#include <mizuiro/color/types/channel_value.hpp>
#include <mizuiro/color/object.hpp>
#include <mizuiro/color/output.hpp>
#include <exception>
#include <cstddef>
#include <iostream>
#include <ostream>

namespace
{
struct view_visitor
{
public:
	typedef
	void
	result_type;

	template<typename View>
	result_type
	operator()(
		View const &) const;
};

// TODO: Pattern match on mizuiro::color::homogenous?
template<typename Input>
struct promote_homogenous_channels
{
	typedef
	mizuiro::color::homogenous
	<
		typename boost::promote<typename Input::channel_type>::type,
		typename Input::layout
	>
	type;
};

template<typename Color>
struct set_to_zero_functor
{
FCPPT_NONASSIGNABLE(
	set_to_zero_functor);
public:
	typedef
	void
	result_type;

	explicit
	set_to_zero_functor(
		Color &_target_color)
	:
		target_color_(
			_target_color)
	{
		
	}

	template<typename Channel>
	result_type
	operator()(
		Channel &) const 	
	{
		typedef typename 
		mizuiro::color::types::channel_value<typename Color::format,Channel>::type
		channel_value_type;

		mizuiro::color::set<Channel>(
			target_color_,
			static_cast<channel_value_type>(0));
	}
private:
	Color &target_color_;
};

template<typename Color>
Color const
zero_initialized()
{
	Color result;
	mizuiro::color::for_each_channel<typename Color::format>(
		set_to_zero_functor<Color>(
			result));
	return result;
}

template<typename SourceColor,typename DestColor>
struct addition_functor
{
FCPPT_NONASSIGNABLE(
	addition_functor);
public:
	typedef
	void
	result_type;

	explicit
	addition_functor(
		SourceColor const &_source,
		DestColor &_dest)
	:
		source_(
			_source),
		dest_(
			_dest)
	{
	}

	template<typename Channel>
	result_type
	operator()(
		Channel &) const
	{
		mizuiro::color::set<Channel>(
			dest_,
			mizuiro::color::get<Channel>(
				dest_) +
			mizuiro::color::get<Channel>(
				source_));
	}
private:
	SourceColor const &source_;
	DestColor &dest_;
};

template<typename SourceColor,typename DestColor>
struct division_functor
{
FCPPT_NONASSIGNABLE(
	division_functor);
public:
	typedef
	void
	result_type;

	explicit
	division_functor(
		SourceColor const &_source,
		DestColor &_dest,
		typename SourceColor::format::channel_type const _divisor)
	:
		source_(
			_source),
		dest_(
			_dest),
		divisor_(
			_divisor)
	{
	}

	template<typename Channel>
	result_type
	operator()(
		Channel &) const
	{
		typedef typename
		mizuiro::color::types::channel_value<typename DestColor::format,Channel>::type
		destination_channel_type;

		mizuiro::color::set<Channel>(
			dest_,
			static_cast<destination_channel_type>(
				mizuiro::color::get<Channel>(
					source_)/
				divisor_));
	}
private:
	SourceColor const &source_;
	DestColor &dest_;
	typename SourceColor::format::channel_type const divisor_;
};

template<typename SourceColor,typename DestColor>
addition_functor<SourceColor,DestColor> const
make_addition_functor(
	SourceColor const &source,
	DestColor &dest)
{
	return 
		addition_functor<SourceColor,DestColor>(
			source,
			dest);
}

template<typename View>
view_visitor::result_type
view_visitor::operator()(
	View const &v) const
{
	typedef typename
	View::color_format
	color_format;

	typedef typename
	promote_homogenous_channels<color_format>::type
	promoted_color_format;

	typedef
	mizuiro::color::object<promoted_color_format>
	promoted_color;

	typedef
	mizuiro::color::object<color_format>
	input_color;

	typedef typename
	View::iterator
	iterator;

	promoted_color result(
		zero_initialized<promoted_color>());

	typedef typename
	promoted_color_format::channel_type
	promoted_channel_type;
	
	promoted_channel_type 
		number_of_pixels = 0;
	for(
		iterator current_pixel = v.begin(); 
		current_pixel != v.end(); 
		++current_pixel)
	{
		mizuiro::color::for_each_channel<color_format>(
			make_addition_functor(
				*current_pixel,
				result));
		++number_of_pixels;
	}

	input_color narrowed_result;
	mizuiro::color::for_each_channel<color_format>(
		division_functor<promoted_color,input_color>(
			result,
			narrowed_result,
			number_of_pixels));

	std::cout << "promoted result: " << result << ", number of pixels: " << number_of_pixels << ", result: " << narrowed_result << "\n";
}
}

int 
main(
	int argc,
	char *argv[])
try
{
	if(argc != 2)
	{
		fcppt::io::cerr << FCPPT_TEXT("usage: ") << argv[0] << FCPPT_TEXT(" <image-file>\n");
		return EXIT_FAILURE;
	}

	sge::systems::instance sys(
		sge::systems::list()
			(sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

	sge::image2d::file_ptr loaded_file(
		sys.image_loader().load(
			fcppt::from_std_string(
				argv[1])));

	fcppt::variant::apply_unary(
		view_visitor(),
		sge::image2d::view::sub(
			loaded_file->view(),
			sge::image2d::rect(
				sge::image2d::rect::vector(
					loaded_file->size().w()/2,
					0),
				sge::image2d::dim(
					loaded_file->size().w()/2,
					loaded_file->size().h()))));
}
catch(
	fcppt::exception const &e)
{
	fcppt::io::cout << FCPPT_TEXT("Exception: ") << e.string() << FCPPT_TEXT("\n");
}
