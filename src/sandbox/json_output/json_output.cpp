#include <sge/parse/json/output_tabbed.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>

int
main(
	int,
	char *argv[])
{
	fcppt::io::cout()
		<<
			sge::parse::json::output_tabbed(
				sge::parse::json::parse_file_exn(
					fcppt::from_std_string(
						argv[1])))
		<< FCPPT_TEXT("\n");
}
