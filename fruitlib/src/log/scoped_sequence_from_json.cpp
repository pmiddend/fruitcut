#include <fruitlib/exception.hpp>
#include <fruitlib/log/scoped.hpp>
#include <fruitlib/log/scoped_sequence_from_json.hpp>
#include <fruitlib/log/string_to_location.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <sge/parse/json/get_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/optional_to_exception.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/level_from_string.hpp>


fruitlib::log::scoped_sequence
fruitlib::log::scoped_sequence_from_json(
	fcppt::log::context &_context,
	sge::parse::json::array const &_array
)
try
{
	return
		fcppt::algorithm::map<
			fruitlib::log::scoped_sequence
		>(
			_array.elements,
			[
				&_context
			](
				sge::parse::json::value const &_element
			)
			{
				sge::parse::json::array const &current_pair(
					sge::parse::json::get_exn<
						sge::parse::json::array
					>(
						_element
					)
				);

				if(current_pair.elements.size() != 2)
					throw fruitlib::exception(
						FCPPT_TEXT("The loggers have to be stored in an array of pairs [name,level], this was not the case with at least one logger pair"));

				return
					fcppt::make_unique_ptr_fcppt<
						fruitlib::log::scoped
					>(
						_context,
						fruitlib::log::string_to_location(
							FCPPT_TEXT("sge/")
							+
							sge::parse::json::get_exn<
								fcppt::string
							>(
								current_pair.elements[0]
							),
							FCPPT_TEXT('/')
						),
						fcppt::optional_to_exception(
							fcppt::log::level_from_string(
								sge::parse::json::get_exn<
									fcppt::string
								>(
									current_pair.elements[1]
								)
							),
							[]{
								return
									fruitlib::exception{
										FCPPT_TEXT("Invalid log level.")
									};
							}
						)
					);
			}
		);
}
catch(
	sge::parse::json::invalid_get const &
)
{
	throw
		fruitlib::exception(
			FCPPT_TEXT("The loggers have to be stored in an array of pairs [name,level], this was not the case with at least one logger pair")
		);
}
