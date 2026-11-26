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
	{

	}

	auto create() -> graph_and_vertices;
private:
	std::vector<int> m_vertices_id;

	void attach_vertices(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices);
	void attach_edges(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices);
	auto init_vertices_id() -> std::vector<int>;
	void init_vertices(std::vector<Vertex>& vertices);
	int random_num_gen(int min_number,int max_number);
};