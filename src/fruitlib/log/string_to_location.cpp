#include "string_to_location.hpp"
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/range/numeric.hpp>
#include <vector>

fcppt::log::location const
fruitcut::fruitlib::log::string_to_location(
	fcppt::string const &input,
	fcppt::char_type const delimiter)
{
	typedef
	std::vector<fcppt::string>
	output_vector;

	output_vector parts;

	return
 		boost::accumulate(
			boost::algorithm::split(
				parts,
				input,
				boost::phoenix::arg_names::arg1 == delimiter),
			fcppt::log::location(),
			boost::phoenix::arg_names::arg1 + boost::phoenix::arg_names::arg2);
}