#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/output/tabbed_to_string.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>


int
main(
	int,
	char *argv[])
{
	fcppt::io::cout()
		<<
			sge::parse::json::output::tabbed_to_string(
				sge::parse::json::parse_file_exn(
					fcppt::from_std_string(
						argv[1])))
		<< FCPPT_TEXT("\n");
}
