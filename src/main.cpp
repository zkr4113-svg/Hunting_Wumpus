#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>

import std;
import vertex;
import room_generator;
import game_manager;

int main()
{
	RoomGen room_generator;
	auto [graph_map, vertices] = room_generator.create();

	GameManager game_manager(graph_map, vertices);

	try
	{
		game_manager.game_loop();
	}
	catch (const std::exception& e)
	{
		std::println(std::cerr, "ERROR: {}", e.what());
	}
	
}
