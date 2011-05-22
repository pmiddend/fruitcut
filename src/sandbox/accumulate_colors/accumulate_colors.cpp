#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/from_std_string.hpp>
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
		View const &);
};

template<typename View>
view_visitor::result_type
view_visitor::operator()(
	View const &)
{
	
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

	/*
	fcppt::variant::apply_unary(
		view_visitor(),
		loaded_file->view());
	*/
}
catch(
	fcppt::exception const &e)
{
	fcppt::io::cout << FCPPT_TEXT("Exception: ") << e.string() << FCPPT_TEXT("\n");
}
