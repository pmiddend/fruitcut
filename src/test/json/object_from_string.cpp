#include <test/json/object_from_string.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/parse_stream.hpp>
#include <fcppt/string.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/assert.hpp>

sge::parse::json::object const
fruitcut::test::json::object_from_string(
	fcppt::string const &s)
{
	sge::parse::json::object result_object;

	fcppt::io::istringstream input_file(
		s);

	bool const parse_result = 
		sge::parse::json::parse_stream(
			input_file,
			result_object);

	FCPPT_ASSERT(
		parse_result);

	return result_object;
}
