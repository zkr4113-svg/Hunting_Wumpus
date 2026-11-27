module;
#include <graph/container/undirected_adjacency_list.hpp>
#include <graph/generators/barabasi_albert.hpp>
export module room_generator;
import std;
import vertex;


struct graph_and_vertices
{
	graph::container::undirected_adjacency_list<int, Vertex> graph;
	std::vector<Vertex> vertices;
};

export class RoomGen
{
public:
	RoomGen()
		:m_vertices_id{ init_vertices_id() }
	{ }
	
	//get the graph and vertices array to the game manager
	auto create() -> graph_and_vertices;
private:
	//in order to init every vertex's id.
	std::vector<int> m_vertices_id;
	//attach vertices to the graph 
	void attach_vertices(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices);
	//attach edges to the graph
	void attach_edges(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices);
	//init m_vertices_id
	auto init_vertices_id() -> std::vector<int>;
	//according m_vertices_id to set states of every vertex of the graph, use vertex's id to locate and change the specific position.
	void init_vertices(std::vector<Vertex>& vertices);
	//generate random id of the graph's vertex id, in order to set states randomly.
	int random_num_gen(int min_number,int max_number);
};