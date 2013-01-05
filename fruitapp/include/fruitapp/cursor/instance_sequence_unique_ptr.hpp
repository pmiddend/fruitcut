#ifndef FRUITAPP_CURSOR_INSTANCE_SEQUENCE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_SEQUENCE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/cursor/instance_sequence_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace cursor
{
typedef
std::unique_ptr<fruitapp::cursor::instance_sequence>
instance_sequence_unique_ptr;
}
}

#endif
