module;
#include <graph/container/undirected_adjacency_list.hpp>
#include <graph/generators/barabasi_albert.hpp>
module room_generator;

void RoomGen::attach_vertices(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices)
{
	for (int i{ }; i < vertices.size(); ++i)
	{
		graph.add_vertex(vertices[i]);
	}
}

void RoomGen::attach_edges(graph::container::undirected_adjacency_list<int, Vertex>& graph, std::span<Vertex> vertices)
{
	using id_type = std::uint32_t;
	const auto n{ static_cast<id_type>(vertices.size()) };
	
	auto edges = graph::generators::barabasi_albert(n, id_type{ 2 }, std::random_device{ }());

	for (const auto& e : edges)
	{
		if (e.source_id < e.target_id)
		{
			graph.add_edge(e.source_id, e.target_id);
		}
	}
}

int RoomGen::random_num_gen(int min_number, int max_number)
{
	std::random_device seed;
	std::mt19937 engine{ seed() };
	std::uniform_int_distribution<int> distribution{ min_number,max_number };
	int result{ distribution(engine) };
	return result;
}


auto RoomGen::init_vertices_id() -> std::vector<int>
{
	const int graph_size_min{ 5 }, graph_size_max{ 20 };

	int graph_size = random_num_gen(graph_size_min, graph_size_max);

	auto result{ std::views::iota(0,graph_size-1) | std::ranges::to<std::vector>() };

	return result;
}

void RoomGen::init_vertices(std::vector<Vertex>& vertices)
{
	for (int i{ }; i < m_vertices_id.size(); ++i)
	{
		vertices[i].set_id(m_vertices_id[i]);
	}

	int random_index_max{ static_cast<int>(vertices.size() - 1) };
	int random_state_max{ std::to_underlying(Vertex::StateFlags::bug_state) }, random_state_min{ std::to_underlying(Vertex::StateFlags::hole_state) };

	std::vector<int> player_wumpus_positions;
	std::mt19937 gen{ std::random_device{ }() };
	std::ranges::sample(m_vertices_id, std::back_inserter(player_wumpus_positions), 2, gen);

	auto player_position{ player_wumpus_positions[0] }, wumpus_position{ player_wumpus_positions[1] };

	vertices[player_position].set_state(Vertex::StateFlags::player_state);
	vertices[wumpus_position].set_state(Vertex::StateFlags::wumpus_state);

	if (vertices.size() > 8)
	{
		unsigned cnt{ };
		int old_position{ };
		while (cnt < (vertices.size() / 3))
		{
			auto position{ random_num_gen(0,random_index_max) };
			if (position != player_position && position != wumpus_position && position != old_position)
			{
				auto random_state{ random_num_gen(random_state_min,random_state_max) };
				vertices[position].set_state(random_state);
			}
			cnt++;
			old_position = position;
		}
	}

	std::println(std::cerr, "player_and_wumpus positons: {:n}", player_wumpus_positions);
}


auto RoomGen::create() -> graph_and_vertices
{
	std::println("{:n}", m_vertices_id);

	std::vector<Vertex> vertices(m_vertices_id.size(), Vertex{ 0,0 });
	init_vertices(vertices);
	graph::container::undirected_adjacency_list<int, Vertex> graph_map;

	attach_vertices(graph_map, vertices);
	attach_edges(graph_map, vertices);

	return { graph_map,vertices };
}