#ifndef FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_IMPL_HPP_INCLUDED
#define FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_IMPL_HPP_INCLUDED

#include <fruitlib/basic_scoped_ostream_file_redirection_decl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <stdexcept>
#include <fcppt/config/external_end.hpp>


template<typename Char,typename CharTraits>
fruitlib::basic_scoped_ostream_file_redirection<Char,CharTraits>::basic_scoped_ostream_file_redirection(
	ostream &_stream,
	boost::filesystem::path const &_file)
:
	stream_(
		_stream),
	file_(
		_file),
	old_streambuf_(
		stream_.rdbuf())
{
	if(!file_.is_open())
		throw
			std::runtime_error(
				"Couldn't open redirection file "+
				_file.string<std::string>());

	stream_.rdbuf(
		file_.rdbuf());
}

template<typename Char,typename CharTraits>
fruitlib::basic_scoped_ostream_file_redirection<Char,CharTraits>::~basic_scoped_ostream_file_redirection()
{
	if(stream_.rdbuf() != old_streambuf_)
		stream_.rdbuf(
			old_streambuf_);
}

#endif

