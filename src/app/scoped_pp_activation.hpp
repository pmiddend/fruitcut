#ifndef FRUITCUT_APP_SCOPED_PP_ACTIVATION_HPP_INCLUDED
#define FRUITCUT_APP_SCOPED_PP_ACTIVATION_HPP_INCLUDED

#include "postprocessing_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class scoped_pp_activation
{
FCPPT_NONCOPYABLE(
	scoped_pp_activation);
public:
	explicit
	scoped_pp_activation(
		app::postprocessing &,
		bool);

	~scoped_pp_activation();
private:
	app::postprocessing &pp_;
	bool old_activation_;
};
}
}

#endif
