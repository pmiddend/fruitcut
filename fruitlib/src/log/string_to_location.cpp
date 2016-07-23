#include <fruitlib/log/string_to_location.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/fold.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/name.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


fcppt::log::location
fruitlib::log::string_to_location(
	fcppt::string const &input,
	fcppt::char_type const delimiter)
{
	typedef
	std::vector<fcppt::string>
	output_vector;

	output_vector parts;

	return
		fcppt::algorithm::fold(
			boost::algorithm::split(
				parts,
				input,
				[
					delimiter
				](
					fcppt::char_type const _ch
				)
				{
					return
						_ch
						==
						delimiter;
				}
			),
			fcppt::log::location(),
			[](
				fcppt::string const &_cur,
				fcppt::log::location &&_state
			)
			{
				return
					_state
					/
					fcppt::log::name{
						_cur
					};
			}
		);
}
