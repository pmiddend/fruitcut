#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <iterator>

int main() {}
#if 0
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

class ternary
:
	public base
{
public:
	virtual sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr,
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
		sge::renderer::texture_ptr);
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
		sge::renderer::texture_ptr);
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
		sge::renderer::texture_ptr);
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
		sge::renderer::texture_ptr);
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
		sge::renderer::texture_ptr);
};
}

class filter_wrapper
{
public:
	explicit
	filter_wrapper(
		filters::base &_filter,
		std::string const &_name)
	:
		filter_(
			&_filter),
		name_(
			_name)
	{
	}

	std::string const name() const { return name_; }
private:
	filters::base *filter_;
	std::string name_;
};

typedef 
boost::adjacency_list
<
	boost::vecS, 
	boost::vecS, 
	boost::directedS,
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
boost::graph_traits<graph>::in_edge_iterator
in_edge_iterator;

typedef 
std::map
<
	vertex_descriptor,
	filter_wrapper
>
vertex_map;

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
{
	typedef 
	std::pair<int, int> 
	edge;

	enum { filter_ssaa, filter_highlight, filter_blur, filter_add, filter_desaturate };

	filters::ssaa ssaa_;
	filters::highlight highlight_;
	filters::blur blur_;
	filters::add add_;
	filters::desaturate desaturate_;

	graph g;

	vertex_map vertex_to_filter;

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
		vertex_descriptor const v,
		sorted)
	{
		std::pair<in_edge_iterator,in_edge_iterator> ie = 
			boost::in_edges(
				v,
				g);

		std::cout << "Vertex " << vertex_to_filter[v].name() << " has predecessors: ";
		for (; ie.first != ie.second; ++ie.first)
			std::cout << vertex_to_filter[boost::source(*ie.first)].name() << "\n";
		// - get filter_wrapper 'f' for current element
		// - get predecessors
		// - if none, f.apply(root_texture());
		// - if one (called 'g'), f.apply(g.texture())
		// - if two (called 'g', 'h'), f.apply(g.texture(),h.texture());
		// - and so on
	}
	/*
	ssaa_filter ssaa;
	highlight_filter highlight;
	blur_filter blur;
	add_filter add;
	desaturate_filter desaturate;

	depgraph.insert(
		highlight,
		ssaa);

	depgraph.insert(
		blur,
		highlight);

	depgraph.insert(
		add,
		blur);

	depgraph.insert(
		add,
		ssaa);

	depgraph.insert(
		desaturate,
		add);

	render(
		desaturate.texture());
	*/
}
#endif
