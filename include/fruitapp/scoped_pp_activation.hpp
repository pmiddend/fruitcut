#ifndef FRUITAPP_SCOPED_PP_ACTIVATION_HPP_INCLUDED
#define FRUITAPP_SCOPED_PP_ACTIVATION_HPP_INCLUDED

#include <fruitapp/postprocessing_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
class scoped_pp_activation
{
FCPPT_NONCOPYABLE(
	scoped_pp_activation);
public:
	explicit
	scoped_pp_activation(
		fruitapp::postprocessing &,
		bool);

	~scoped_pp_activation();
private:
	fruitapp::postprocessing &pp_;
	bool old_activation_;
};
}

#endif
