#ifndef FRUITAPP_FRUIT_CUT_CONTEXT_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_CONTEXT_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fcppt/unique_ptr.hpp>

namespace fruitapp
{
namespace fruit
{
typedef
fcppt::unique_ptr<fruit::cut_context>
cut_context_unique_ptr;
}
}

#endif

