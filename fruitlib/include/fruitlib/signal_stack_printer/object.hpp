#ifndef FRUITLIB_SIGNAL_STACK_PRINTER_OBJECT_HPP_INCLUDED
#define FRUITLIB_SIGNAL_STACK_PRINTER_OBJECT_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace signal_stack_printer
{
class object_impl;

class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object();

	~object();
private:
	std::unique_ptr<signal_stack_printer::object_impl> const impl_;
};
}
}

#endif

