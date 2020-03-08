#include<iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;
using namespace std;

struct DotVertex {
	
	std::string label;
	
};

struct DotEdge {
	std::string label;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, DotVertex, DotEdge> graph_t;

int main() {
	graph_t graphviz;
	boost::dynamic_properties dp(boost::ignore_other_properties);

//	dp.property("node_id", boost::get(&DotVertex::name, graphviz));
	dp.property("label", boost::get(&DotVertex::label, graphviz));
	//dp.property("peripheries", boost::get(&DotVertex::peripheries, graphviz));
	dp.property("label", boost::get(&DotEdge::label, graphviz));
	std::ifstream dot("C:/Users/pxg131330/Downloads/acmart-master/input.dot");
	bool status = boost::read_graphviz(dot, graphviz, dp);
	if (status)
		cout << 255;

	system("pause");
	return 0;

}
