#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>

import std;
import vertex;
import room_generator;
import game_manager;

namespace
{
	void init_console()
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD mode{ };
		if (GetConsoleMode(out, &mode))
			SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}

	void clear_screen()
	{
		std::print("\x1b[2J\x1b[3J\x1b[H");
		std::fflush(stdout);
	}

	void pause()
	{
		std::print("Press Enter to continue...");
		std::string dummy;
		std::getline(std::cin, dummy);
	}



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
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif // _WIN32

	RoomGen room_generator;
	auto [graph_map, vertices] = room_generator.create();

	GameManager game_manager(graph_map, vertices);

	try
	{
		while (true)
		{
			clear_screen();
			game_manager.game_menu();
			
			int op = read_choice();
			switch (op)
			{
			case 1:
				clear_screen();
				game_manager.game_loop();
				break;
			case 2:
				return 0;
			default:
				break;
			}

			pause();
		}
	}
	catch (const std::exception& e)
	{
		std::println(std::cerr, "ERROR: {}", e.what());
		return 1;
	}
}