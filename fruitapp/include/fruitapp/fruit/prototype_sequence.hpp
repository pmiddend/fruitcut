#ifndef FRUITAPP_FRUIT_PROTOTYPE_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_SEQUENCE_HPP_INCLUDED

#include <fruitapp/fruit/prototype.hpp>
#include <fcppt/unique_ptr_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
std::vector
<
	fcppt::unique_ptr
	<
		prototype
	>
>
prototype_sequence;
}
}

#endif
