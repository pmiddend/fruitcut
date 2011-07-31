#include "../fruitlib/delayed_ptr_sequence.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/make_unique_ptr.hpp>

BOOST_AUTO_TEST_CASE(ptr_sequence)
{
	fruitlib::delayed_ptr_sequence<int,boost::ptr_vector> seq;
	seq.push_back(
		fcppt::make_unique_ptr<int>());
	BOOST_CHECK(
		seq.empty() && seq.size() == 0u);
	seq.update();
	BOOST_CHECK(
		!seq.empty() && seq.size() == 1u);
	seq.erase(
		seq.front());
	BOOST_CHECK(
		!seq.empty() && seq.size() == 1u);
	seq.update();
	BOOST_CHECK(
		seq.empty() && seq.size() == 0u);
}
