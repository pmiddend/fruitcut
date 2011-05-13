#include "config_wrapper.hpp"
#include "name.hpp"
#include "../exception.hpp"
#include "../json/merge_trees.hpp"
#include "../json/process_option.hpp"
#include "../media_path.hpp"
#include <sge/config/find_config_path.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <iostream>
#include <ostream>
#include <string>

namespace
{
fcppt::optional<fcppt::filesystem::path> const
user_config_file()
{
	fcppt::filesystem::path const final_name = 
		sge::config::find_config_path(
			fruitcut::app::name())/
		FCPPT_TEXT("config.json");

	return 
		fcppt::filesystem::exists(
			final_name)
		?
			fcppt::optional<fcppt::filesystem::path>(
				final_name)
		:
			fcppt::optional<fcppt::filesystem::path>();
}
}

sge::parse::json::object const
fruitcut::app::config_wrapper(
	int argc,
	char *argv[])
{
	sge::parse::json::object config_file =
		sge::parse::json::parse_file_exn(
			fruitcut::media_path()/FCPPT_TEXT("config.json"));

	if (user_config_file())
		config_file = 
			json::merge_trees(
				config_file,
				sge::parse::json::parse_file_exn(
					*user_config_file()));

	if (argc >= 2 && std::string(argv[1]) == "--help")
	{
		std::cout << 
			"Command line options are of the form:\n\n"
			"foo/bar/baz=qux\n\n"
			"where foo/bar/baz is a sequence of objects in the config.json file.\n"
			"qux can be any json type (strings, arrays, etc.)\n"
			"For example, if config.json looks like this:\n\n"
			"{ \"renderer\" : { \"screen_size\" : [640,480] } }\n\n"
			"You could change the resolution via:\n\n"
			"renderer/screen_size=[1024,768]\n\n"
			"Be aware of two things, though:\n"
			"1. You can use _any_ json type, so it's possible to write\n\n"
			"renderer/screen_size=1.0\n\n"
			"which, of course, makes no sense. Try to use the correct type.\n"
			"2. Be aware of your shell' special characters. For example, in bash\n"
			"to set a json string, you have to write:\n\n"
			"player/name='\"foobar\"'\n\n"
			"It's a good idea to always put the argument in apostrophes.\n";
		throw exception(FCPPT_TEXT("Help was needed"));
	}

	for (int i = 1; i < argc; ++i)
		json::process_option(
			config_file,
			fcppt::from_std_string(
				argv[i]));

	return config_file;
}
