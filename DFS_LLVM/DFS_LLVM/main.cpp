#include<iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>


using namespace boost;
using namespace std;

struct DotVertex {
	
	std::string label;
	
};

struct DotEdge {
	std::string label;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, DotVertex, DotEdge> graph_t;
//typedef boost::graph_traits<graph_t>::vertex_descriptor DotVertex;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;

/*
class MyVisitor : public boost::default_dfs_visitor {
public:
	MyVisitor() : vv(new std::vector<DotVertex>()) {}

	void discover_vertex(DotVertex v, const graph_t &g) const {
		vv->push_back(v);
		return;
	}

	std::vector<DotVertex> &GetVector() const { return *vv; }

private:
	boost::shared_ptr<std::vector<DotVertex> > vv;
};
*/

class MyVisitor : public boost::default_dfs_visitor {
public:
	MyVisitor() : vv(new std::vector<DotVertex>()) {}

	void discover_vertex(vertex_t v, const graph_t& g) { //note the lack of const
		//if (boost::in_degree(v, g) != 0) { //only print the vertices in the connected component (I already did MCC and removed edges so all the extra vertices are isolated)
			std::cerr << g[v].label << std::endl;
			vv->push_back(g[v]);
		//}
		return;
	}
	std::vector<DotVertex>& GetVector() const { return *vv; }
private:
	boost::shared_ptr< std::vector<DotVertex> > vv;
};



int main() {
	graph_t graphviz;
	boost::dynamic_properties dp(boost::ignore_other_properties);

//	dp.property("node_id", boost::get(&DotVertex::name, graphviz));
	dp.property("label", boost::get(&DotVertex::label, graphviz));
	//dp.property("peripheries", boost::get(&DotVertex::peripheries, graphviz));
	dp.property("label", boost::get(&DotEdge::label, graphviz));
	std::ifstream dot("C:/Users/pxg131330/Downloads/acmart-master/cfg.main.dot");
	//auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap);

	auto indexmap = boost::get(boost::vertex_index, graphviz);
	auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap);


	bool status = boost::read_graphviz(dot, graphviz, dp);
	if (status)
		cout << 255;

	MyVisitor vis;
	//boost::depth_first_search(graphviz, vis, colormap,1);
	boost::depth_first_search(graphviz, boost::visitor(vis));

	std::vector<DotVertex> vctr = vis.GetVector();

	for (auto id : vctr)
		std::cout << id.label << " ";

	system("pause");
	return 0;

}
