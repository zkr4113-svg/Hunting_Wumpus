#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>

import std;
import vertex;
import room_generator;
import game_manager;

namespace
{
	void print_graph(const graph::container::undirected_adjacency_list<int, Vertex>& g,std::span<Vertex> v)
	{
		std::println("Vertices:");
		for (auto [uid, uv] : graph::views::vertexlist(g))
		{
			std::print("{} ", uid);
		}
		std::println("");

		std::println("edges:");
		for (auto [uid, vid, uv] : graph::views::edgelist(g))
		{
			std::println("{} -> {}", uid, vid);
		}

		for (std::uint32_t i{ }; i != v.size(); ++i)
		{
			std::println("id_{} neighbors:", i);
			for (auto [vid, n] : graph::views::neighbors(g,i))
			{
				std::print("{} ", vid);
			}
			std::println("");
		}
	}

	void debug_state(std::span<Vertex> vertices)
	{
		for (const auto& vi : vertices)
		{
			std::println("room id: {}, state: {}", vi.get_id(), vi.get_state().to_string());
		}
	}
}

int main()
{
	RoomGen room_generator;
	auto [graph_map, vertices] = room_generator.create();

	GameManager game_manager(graph_map, vertices);

	//std::println("vertices num: {}", graph::num_vertices(graph_map));
	//std::println("edges num: {}", graph::num_edges(graph_map));

	//print_graph(graph_map, vertices);
	try
	{
		game_manager.game_loop();
	}
	catch (const std::exception& e)
	{
		std::println(std::cerr, "ERROR: {}", e.what());
	}
	

	
	//std::println(std::cerr, "game over");
}
