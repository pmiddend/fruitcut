#ifndef FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_DECL_HPP_INCLUDED
#define FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_DECL_HPP_INCLUDED

#include <fruitlib/basic_scoped_ostream_file_redirection_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <streambuf>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
template<typename Char,typename CharTraits>
class basic_scoped_ostream_file_redirection
{
FCPPT_NONCOPYABLE(
	basic_scoped_ostream_file_redirection);
public:
	typedef
	std::basic_ostream<Char,CharTraits>
	ostream;

	typedef
	boost::filesystem::basic_ofstream<Char,CharTraits>
	ofstream;

	basic_scoped_ostream_file_redirection(
		ostream &,
		boost::filesystem::path const &);

	~basic_scoped_ostream_file_redirection();
private:
	typedef
	std::basic_streambuf<Char,CharTraits>
	streambuf;

	ostream &stream_;
	ofstream file_;
	streambuf * const old_streambuf_;
};
}

#endif

