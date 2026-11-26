module;
#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>
module game_manager;

void GameManager::game_loop()
{
	while (!game_over)
	{
		auto player_position{ where_player(m_vertices) };
		std::println("you are in room {}", player_position);
		std::print("this room can go to: ");
		std::uint32_t point{ };
		for (auto [vid, n] : graph::views::neighbors(m_graph_map, player_position))
		{
			std::print("{} ", vid);
			point = static_cast<std::uint32_t>(vid);
		}

		if (graph::contains_edge(m_graph_map, player_position, point))
		{
			std::println(std::cerr, "the edge exist");
		}
		else
		{
			std::println(std::cerr, "the edge is not exist");
		}

		std::println("");
		std::println("move or shoot?");
		PlayerController player{ };
		std::string cmd;
		std::print(">> ");
		std::getline(std::cin, cmd);
		bool ok = parse_command(cmd, player);

		if (ok)
		{
			player_update(player, m_graph_map, m_vertices);
			wumpus_update(m_graph_map, m_vertices);
			bug_update(m_graph_map, m_vertices);
		}
		else
		{
			std::println(std::cerr, "parse worng");
		}


		check_if_win();
	}
}

std::uint32_t GameManager::random_num_gen(std::uint32_t min_number, std::uint32_t max_number)
{
	std::random_device seed;
	std::mt19937 engine{ seed() };
	std::uniform_int_distribution<std::uint32_t> distribution{ min_number,max_number };
	std::uint32_t result{ distribution(engine) };
	return result;
}

void GameManager::check_if_win()
{
	if (is_wumpus_dead)
	{
		std::println("wumpus is dead");
		game_over = true;
	}

	if (is_player_dead)
	{
		std::println(std::cerr, "player is dead");
		game_over = true;
	}
}

std::uint32_t GameManager::where_player(std::span<Vertex> vertices)
{
	std::uint32_t player_position{ 1000 };

	for (std::size_t i{ }; i < vertices.size(); ++i)
	{
		if (vertices[i].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
		{
			player_position = static_cast<uint32_t>(i);
		}
	}

	if (player_position == 1000)
	{
		throw std::runtime_error{ "player is not exist." };
	}

	return player_position;
}

void GameManager::random_move_logic(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices,
	Vertex::StateFlags flag)
{
	std::uint32_t position{ };
	for (std::size_t i{ }; i < vertices.size(); ++i)
	{
		if (vertices[i].get_state().test(std::to_underlying(flag)))
		{
			position = static_cast<std::uint32_t>(i);
		}
	}

	std::vector<std::uint32_t> neighbors;
	for (auto [vid, n] : graph::views::neighbors(g, position))
	{
		neighbors.push_back(static_cast<std::uint32_t>(vid));
	}

	auto where_to_go{ random_num_gen(0,static_cast<std::uint32_t>(neighbors.size() - 1)) };

	vertices[where_to_go].set_state(flag);
	vertices[position].flip_state(flag);
}

void GameManager::wumpus_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices)
{
	if (!is_wumpus_dead)
	{
		random_move_logic(g, vertices, Vertex::StateFlags::wumpus_state);
	}
}

void GameManager::bug_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices)
{
	random_move_logic(g, vertices, Vertex::StateFlags::bug_state);

	for (std::size_t i{ }; i < vertices.size(); ++i)
	{
		if (vertices[i].get_state().test(std::to_underlying(Vertex::StateFlags::bug_state)) &&
			vertices[i].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
		{
			random_move_logic(g, vertices, Vertex::StateFlags::player_state);
		}
	}
}

void GameManager::player_update(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices)
{
	auto player_position{ where_player(vertices) };
	if (st.player_action == PlayerAction::move)
	{
		if (graph::contains_edge(g, static_cast<uint32_t>(st.room_id), player_position))
		{
			std::println(std::cerr, "the edge is not exist");
			player_move_logic(st, vertices,player_position);
		}
		else
		{
			std::println("I can't go there");
		}
	}
	else if (st.player_action == PlayerAction::shoot)
	{
		player_shoot_logic(st, g, vertices, player_position);
	}
}

void GameManager::player_move_logic(const PlayerController& st, std::vector<Vertex>& vertices, std::uint32_t player_position)
{
	vertices[st.room_id].set_state(Vertex::StateFlags::player_state);
	vertices[player_position].flip_state(Vertex::StateFlags::player_state);

	if (vertices[st.room_id].get_state().test(std::to_underlying(Vertex::StateFlags::hole_state)) &&
		vertices[st.room_id].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
	{
		is_player_dead = true;
	}
}

void GameManager::player_shoot_logic(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices, std::uint32_t player_position)
{
	std::vector<bool> three_rooms_states;
	three_rooms_states.reserve(3);

	three_rooms_states.push_back(graph::contains_edge(g, player_position, st.room_ids[0]));
	for (std::size_t i{ 1 }; i < st.room_ids.size() - 1; ++i)
	{
		three_rooms_states.push_back(graph::contains_edge(g, st.room_ids[i - 1], st.room_ids[i]));
	}

	bool is_three_rooms_through{ std::ranges::all_of(three_rooms_states,[](const auto elem) { return elem == true; }) };

	if (is_three_rooms_through)
	{
		for (std::size_t i{ }; i < st.room_ids.size(); ++i)
		{
			if (vertices[st.room_ids[i]].get_state().test(std::to_underlying(Vertex::StateFlags::wumpus_state)))
			{
				is_wumpus_dead = true;
				break;
			}

			if (vertices[st.room_ids[i]].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
			{
				is_player_dead = true;
				break;
			}
		}
	}
	else
	{
		if (three_rooms_states[0])
		{
			if (vertices[st.room_ids[0]].get_state().test(std::to_underlying(Vertex::StateFlags::wumpus_state)))
			{
				is_wumpus_dead = true;
			}

			if (vertices[st.room_ids[0]].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
			{
				is_player_dead = true;
			}
		}
		else
		{
			for (std::size_t i{ }; i < vertices.size(); ++i)
			{
				if (vertices[st.room_ids[i]].get_state().test(std::to_underlying(Vertex::StateFlags::wumpus_state)))
				{
					is_wumpus_dead = true;
					break;
				}

				if (vertices[st.room_ids[i]].get_state().test(std::to_underlying(Vertex::StateFlags::player_state)))
				{
					is_player_dead = true;
					break;
				}
			}
		}

	}

}
