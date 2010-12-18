#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/algorithm/map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace
{
namespace filters
{
class base
{
public:
	virtual ~base() {}
};

class unary
:
	public base
{
public:
	virtual sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr) = 0;
};

class binary
:
	public base
{
public:
	virtual sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr,
		sge::renderer::texture_ptr) = 0;
};

class ssaa 
:
	public unary
{
FCPPT_NONCOPYABLE(ssaa)
public:
	ssaa() {}

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr) { std::cout << "ssaa apply\n";  return sge::renderer::texture_ptr();}
};

class blur 
:
	public unary
{
FCPPT_NONCOPYABLE(blur)
public:
	blur() {}

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr) {std::cout << "blur apply\n"; return sge::renderer::texture_ptr();}
};

class highlight 
:
	public unary
{
FCPPT_NONCOPYABLE(highlight)
public:
	highlight() {}

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr) {std::cout << "highlight apply\n"; return sge::renderer::texture_ptr();}
};

class add 
:
	public binary
{
FCPPT_NONCOPYABLE(add)
public:
	add() {}

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr,
		sge::renderer::texture_ptr) {std::cout << "add apply\n"; return sge::renderer::texture_ptr();}
};

class desaturate 
:
	public unary
{
FCPPT_NONCOPYABLE(desaturate)
public:
	desaturate() {}

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr) {std::cout << "desaturate apply\n"; return sge::renderer::texture_ptr();}
};
}

class filter_wrapper
{
public:
	typedef
	std::vector<sge::renderer::texture_ptr>
	texture_sequence;

	explicit
	filter_wrapper()
	:
		filter_(0),
		name_()
	{
	}

	explicit
	filter_wrapper(
		filters::base &_filter,
		std::string const &_name,
		bool _active = true)
	:
		filter_(
			&_filter),
		name_(
			_name),
		active_(
			_active)
	{
	}

	bool 
	active() const 
	{ 
		return active_; 
	}

	sge::renderer::texture_ptr const
	apply(
		texture_sequence const &textures)
	{
		FCPPT_ASSERT_MESSAGE(
			!textures.empty(),
			FCPPT_TEXT("A filter didn't get any input textures!"));

		FCPPT_ASSERT_MESSAGE(
			textures.size() <= static_cast<texture_sequence::size_type>(2),
			FCPPT_TEXT("Currently, only unary and binary filters are allowed"));

		sge::renderer::texture_ptr const first = 
			*textures.begin();

		if (textures.size() == static_cast<texture_sequence::size_type>(1))
		{
			filters::unary &f = 
				dynamic_cast<filters::unary &>(
					*filter_);
			if (!active_)
				return first;
			return 
				f.apply(
					first);
		}

		if (textures.size() == static_cast<texture_sequence::size_type>(2))
		{
			sge::renderer::texture_ptr const second = 
				*boost::next(
					textures.begin());

			filters::binary &f = 
				dynamic_cast<filters::binary &>(
					*filter_);

			if (!active_)
			{
				FCPPT_ASSERT_MESSAGE(
					&(*first) == &(*second),
					FCPPT_TEXT("A deactivated binary filter didn't get the same two textures.\nProbably means that the dependencies aren't resolved correctly.\nTime to debug!"));
				
				return first;
			}
			
			return 
				f.apply(
					first,
					second);
		}
	}
private:
	filters::base *filter_;
	std::string name_;
	bool active_;
};

typedef 
boost::adjacency_list
<
	boost::vecS, 
	boost::vecS, 
	boost::bidirectionalS,
	boost::property
	<
		boost::vertex_name_t,
		filter_wrapper
	>
> 
graph;

typedef
boost::graph_traits<graph>::vertex_descriptor
vertex_descriptor;

typedef
boost::graph_traits<graph>::edge_descriptor
edge_descriptor;

typedef
boost::graph_traits<graph>::in_edge_iterator
in_edge_iterator;

typedef 
std::map
<
	vertex_descriptor,
	filter_wrapper
>
vertex_map;

typedef 
std::map
<
	vertex_descriptor,
	sge::renderer::texture_ptr
>
texture_map;

vertex_descriptor
add_vertex(
	graph &g,
	vertex_map &vm,
	filter_wrapper const &f)
{
	vertex_descriptor r = 
		boost::add_vertex(
			g);

	vm[r] = f;

	return r;
}
}

int main()
try
{
	filters::ssaa ssaa_;
	filters::highlight highlight_;
	filters::blur blur_;
	filters::add add_;
	filters::desaturate desaturate_;

	graph g;

	vertex_map vertex_to_filter;
	texture_map filter_result;

	vertex_descriptor 
		ssaa_vertex = 
			add_vertex(
				g,
				vertex_to_filter,
				filter_wrapper(
					ssaa_,
					"ssaa")),
		highlight_vertex = 
			add_vertex(
				g,
				vertex_to_filter,
				filter_wrapper(
					highlight_,
					"highlight")),
		blur_vertex = 
			add_vertex(
				g,
				vertex_to_filter,
				filter_wrapper(
					blur_,
					"blur")),
		add_vertex_ = 
			add_vertex(
				g,
				vertex_to_filter,
				filter_wrapper(
					add_,
					"add")),
		desaturate_vertex = 
			add_vertex(
				g,
				vertex_to_filter,
				filter_wrapper(
					desaturate_,
					"desaturate"));

	boost::add_edge(
		ssaa_vertex,
		highlight_vertex,
		g);

	boost::add_edge(
		ssaa_vertex,
		add_vertex_,
		g);

	boost::add_edge(
		highlight_vertex,
		blur_vertex,
		g);

	boost::add_edge(
		blur_vertex,
		add_vertex_,
		g);

	boost::add_edge(
		add_vertex_,
		desaturate_vertex,
		g);
	
	typedef
	std::vector<vertex_descriptor>
	vertex_vector;

	vertex_vector sorted;

	boost::topological_sort(
		g,
		std::back_inserter(
			sorted));

	BOOST_FOREACH(
		vertex_descriptor current_vertex,
		boost::adaptors::reverse(
			sorted))
	{
		std::pair<in_edge_iterator,in_edge_iterator> ie = 
			boost::in_edges(
				current_vertex,
				g);

		filter_wrapper::texture_sequence textures;

		if (ie.first == ie.second)
		{
			sge::renderer::texture_ptr rendered_scene;
			textures.push_back(
				rendered_scene);
		}
	
		BOOST_FOREACH(
			edge_descriptor const &e,
			ie)
			textures.push_back(
				filter_result[
					boost::source(
						e,
						g)]);

		filter_result[current_vertex] = 
			vertex_to_filter[current_vertex].apply(
				textures);
	}
}
catch (fcppt::exception const &e)
{
	std::cout << e.string() << "\n";
}
