#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>

import std;
import vertex;
import room_generator;
import game_manager;

namespace
{
	int read_choice()
	{
		std::string line;
		for (;;)
		{
			std::print(">> ");
			if (!std::getline(std::cin, line))
			{
				throw std::runtime_error{ "input error" };
			}

			int op{ };
			auto result = std::from_chars(line.data(), line.data() + line.size(), op);
			if (result.ec == std::errc{ } && result.ptr == line.data() + line.size())
			{
				return op;
			}

			std::println("wrong input,please input a number, without any blanks.");
		}
	}
}


int main()
{
	RoomGen room_generator;
	auto [graph_map, vertices] = room_generator.create();

	GameManager game_manager(graph_map, vertices);

	try
	{
		while (true)
		{
			game_manager.game_menu();
			
			int op = read_choice();
			switch (op)
			{
			case 1:
				system("cls");
				game_manager.game_loop();
				break;
			case 2:
				return 0;
			default:
				break;
			}

			system("pause");
		}
	}
	catch (const std::exception& e)
	{
		std::println(std::cerr, "ERROR: {}", e.what());
	}
}