#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/rendering/sort_fruits.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <iterator>
#include <fcppt/config/external_end.hpp>


namespace
{
bool
sort_functor(
	fruitapp::fruit::object const * const left,
	fruitapp::fruit::object const * const right)
{
	return
		&(left->prototype()) < &(right->prototype());
}
}

void
fruitapp::fruit::rendering::sort_fruits(
	fruitapp::fruit::object_sequence const &_fruits,
	fruitapp::fruit::rendering::fruit_pointer_sequence &_fruit_pointers)
{
	FCPPT_ASSERT_PRE(
		_fruit_pointers.empty());

	// TODO: map
	_fruit_pointers.reserve(
		_fruits.size());

	for(
		auto const &fruit
		:
		_fruits
	)
		_fruit_pointers.push_back(
			fruit.get_pointer());

	std::sort(
		_fruit_pointers.begin(),
		_fruit_pointers.end(),
		&sort_functor);
}
