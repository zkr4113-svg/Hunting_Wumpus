module;
#include <graph/graph.hpp>
#include <graph/container/undirected_adjacency_list.hpp>
export module game_manager;
import std;
import vertex;
import command;

export class GameManager
{
public:
	GameManager(graph::container::undirected_adjacency_list<int, Vertex> g, std::vector<Vertex> vertices)
		:m_graph_map{ std::move(g) }, m_vertices{ std::move(vertices) }
	{

	}

	void game_loop();


private:
	graph::container::undirected_adjacency_list<int, Vertex> m_graph_map;
	std::vector<Vertex> m_vertices;
private:
	bool game_over = false;
	bool is_wumpus_dead = false;
	bool is_player_dead = false;
private:
	std::uint32_t random_num_gen(std::uint32_t min_number, std::uint32_t max_number);
	std::uint32_t where_player(std::span<Vertex> v);
	void check_if_win();
	void random_move_logic(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices,
		Vertex::StateFlags flag);
private:
	void wumpus_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
	void bug_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
	void player_update(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
private:
	void player_move_logic(const PlayerController& st, std::vector<Vertex>& vertices, std::uint32_t player_position);
	void player_shoot_logic(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices, std::uint32_t player_position);
private:
	void print_graph(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::span<Vertex> v);
	void debug_state(std::span<Vertex> vertices);
};