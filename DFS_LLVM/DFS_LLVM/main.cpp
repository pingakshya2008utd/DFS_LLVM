#include<iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


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
			///std::cerr << g[v].label << std::endl;
			vv->push_back(g[v]);
		//}
		return;
	}
	std::vector<DotVertex>& GetVector() const { return *vv; }
private:
	boost::shared_ptr< std::vector<DotVertex> > vv;
};


int main() {

	vector <string> dot_file_path_vec;
	std::string path= "O:/ultimate_new_ave8_timed/";
	std::string ext=".dot";
	for (auto& p : fs::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext) {

			string p_str = to_string(p);
			if (p_str.find("cfg.ave8.dot") != string::npos) {
				dot_file_path_vec.push_back(p_str);
				//std::cout << p_str << '\n';
			}

		}
			
	}
	
	
	
	//vector <string> func_names{ "quantl", "encode", "decode", "reset", "upzero", "adpcm_main" };
	vector <string> func_names{  "ave8" };
	file_counter = 1;

	for(int i=0; i<dot_file_path_vec.size(); i++){
	//for (int i = 72; i < 73; i++) {
		string directory_name = "total_new_solution " + to_string(i);
		cout << directory_name << endl;
		for (int j = 0; j < func_names.size(); j++) {
			file_counter++;
			//string dot_file_name = "O:/total_new_apdcm/total_new_solution"+to_string(i)+"/cfg." + func_names[j] + ".dot";
			string dot_file_name = dot_file_path_vec[i];
			replace(dot_file_name.begin(), dot_file_name.end(), '\\', '/');
			cout << dot_file_name << endl;

			bool exist = std::experimental::filesystem::exists(dot_file_name);

			if (!exist)
				continue;

			func_name_for_res = func_names[j];
			file_counter = i;
			string write_file_str = dot_file_name;
			write_file_str.erase(write_file_str.length() - 12);
			result_file_name = write_file_str  + func_names[j] + "_data.res";
		//	cout << result_file_name << endl;
			
			//string result_file_name = "O:/total_new_apdcm/total_new_solution"+to_string(i)+"/" + func_names[j] + "_data.res";
			graph_t graphviz;
			boost::dynamic_properties dp(boost::ignore_other_properties);

			//	dp.property("node_id", boost::get(&DotVertex::name, graphviz));
			dp.property("label", boost::get(&DotVertex::label, graphviz));
			//dp.property("peripheries", boost::get(&DotVertex::peripheries, graphviz));
			dp.property("label", boost::get(&DotEdge::label, graphviz));
			//std::ifstream dot("O:/total_new_apdcm/total_new_solution98/cfg.quantl.dot");
			std::ifstream dot(dot_file_name);
			//auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap);

			auto indexmap = boost::get(boost::vertex_index, graphviz);
			auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap);


			bool status = boost::read_graphviz(dot, graphviz, dp);
			if (status)
				cout << status<<endl;



			MyVisitor vis;
			//boost::depth_first_search(graphviz, vis, colormap,1);
			boost::depth_first_search(graphviz, boost::visitor(vis));

			std::vector<DotVertex> vctr = vis.GetVector();



			/*
			for (auto id : vctr)
				std::cout << id.label << " ";
			*/


	////////////////////// new comment starts here//////////////////////////////////////////////

	
			vector <int> indegree_vec, outdegree_vec;
			std::ofstream outfile(result_file_name, std::ios_base::app);

			for (auto v : make_iterator_range(vertices(graphviz))) {
				//	cout << graphviz[v].label << " has " << degree(v, graphviz) << " neighbor(s): ";
				outdegree_vec.push_back((int)out_degree(v, graphviz));
				indegree_vec.push_back((int)in_degree(v, graphviz));


			}

			int max_outedges = *max_element(outdegree_vec.begin(), outdegree_vec.end());
			int max_inedges = *max_element(indegree_vec.begin(), indegree_vec.end());
			int total_nodes = vctr.size();

			//	cout << " max number of outgoing edges = " << max_outedges << endl;
				//cout << " max number of incoming edges = " << max_inedges << endl;
				//cout << " total number of nodes= " << total_nodes << endl;


			outfile << " max number of outgoing edges = " << max_outedges << endl;
			outfile << " max number of incoming edges = " << max_inedges << endl;
			outfile << " total number of nodes= " << total_nodes << endl;

			outdegree_vec.clear();
			indegree_vec.clear();
			vctr.clear();
			outfile.close();
			
		}
	}



	system("pause");
	return 0;

}
