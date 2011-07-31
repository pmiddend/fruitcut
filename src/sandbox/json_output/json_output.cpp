#include <fruitlib/json/output_tabbed.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>

int 
main(
	int,
	char *argv[])
{
	fcppt::io::cout 
		<< 
			fruitlib::json::output_tabbed(
				sge::parse::json::parse_file_exn(
					fcppt::from_std_string(
						argv[1]))) 
		<< FCPPT_TEXT("\n");
}
