#ifndef FRUITCUT_APP_CONFIG_VARIABLES_HPP_INCLUDED
#define FRUITCUT_APP_CONFIG_VARIABLES_HPP_INCLUDED

#include "../fruitlib/json/user_config_variable.hpp"
#include "fruit/area.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/audio/scalar.hpp>

namespace fruitcut
{
namespace app
{
class config_variables
{
FCPPT_NONCOPYABLE(
	config_variables);
public:
	typedef
	fruitlib::json::user_config_variable<sge::audio::scalar>
	audio_variable;

	typedef
	fruitlib::json::user_config_variable<fruit::area::value_type>
	splatter_factor_variable;

	explicit
	config_variables(
		sge::parse::json::object const &global_config,
		sge::parse::json::object &user_config);

	audio_variable &
	music_volume();

	audio_variable &
	effects_volume();

	splatter_factor_variable &
	splatter_count_to_area_factor();

	~config_variables();
private:
	// This fixes a subtlety: The json::user_config_variables sync
	// themselves with the json tree in their destructor. Thus, if we
	// want to write the "latest" version of the json tree to a file, we
	// need to do it after the destructors of all the variables. This is
	// why we introduce this variable.
	struct destructor_write_hack
	{
		destructor_write_hack(
			sge::parse::json::object &);

		~destructor_write_hack();

		sge::parse::json::object &user_config_file_;
	};

	destructor_write_hack write_hack_;
	audio_variable music_volume_;
	audio_variable effects_volume_;
	splatter_factor_variable splatter_count_to_area_factor_;
};
}
}

#endif
