#ifndef FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED

#include <fruitapp/highscore/provider/object_base_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
typedef
std::vector
<
	fcppt::unique_ptr
	<
		provider::object_base
	>
>
provider_sequence;
}
}

#endif
