#include <fruitlib/delayed_ptr_sequence.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/test/unit_test.hpp>
#include <fcppt/config/external_end.hpp>


FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

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

FCPPT_PP_POP_WARNING

