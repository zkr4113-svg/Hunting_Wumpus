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

	//the main loop of the game
	void game_loop();
	//the menu of the game
	void game_menu();

private:
	// the cave map
	graph::container::undirected_adjacency_list<int, Vertex> m_graph_map;
	//contains every room properties.
	std::vector<Vertex> m_vertices;
private:
	//to check if game_over to end the main loop
	bool game_over = false;
	//to check if Wumpus dead. if it is true, the game over and judge win.
	bool is_wumpus_dead = false;
	//to check if player dead. if it is true, the game over and judge lose.
	bool is_player_dead = false;
	//to check if player move successfully.
	bool is_player_move_successfully = true;
	//to count the number of arrows.At first the player has 5 arrows.when player action 'shoot' process ssuccessfully,the number substract 1.
	unsigned arrow_count{ 5 };
private:
	std::uint32_t random_num_gen(std::uint32_t min_number, std::uint32_t max_number);
	std::uint32_t where_player(std::span<Vertex> v);
	void check_if_win();
	void random_move_logic(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices,
		Vertex::StateFlags flag);
private:
	//wumpus behavouirs
	void wumpus_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
	//bat behavouirs
	void bat_update(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
	//player behavouris
	void player_update(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices);
private:
	//player actions logic
	//
	//player move
	void player_move_logic(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices, std::uint32_t player_position);
	//player shoot
	void player_shoot_logic(const PlayerController& st, const graph::container::undirected_adjacency_list<int, Vertex>& g, std::vector<Vertex>& vertices, std::uint32_t player_position);
	//
	void random_shoot_logic(std::vector<Vertex>& vertices);
private:
	//helper functions
	// 
	//print the graph's structure.
	void print_graph(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::span<Vertex> v);
	//print the state of every vertex.
	void debug_state(std::span<Vertex> vertices);
	//print the ids of player_position in the graph.
	void print_roads(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::uint32_t player_position) const;
	//print the informations of neighbors of player_position to help player judge the situation.
	void feel_something(const graph::container::undirected_adjacency_list<int, Vertex>& g, const std::vector<Vertex>& vertices, std::uint32_t player_position) const;
	//get the pos neighbors.
	auto get_neighbors(const graph::container::undirected_adjacency_list<int, Vertex>& g, std::uint32_t position) -> std::vector<std::uint32_t>;
	//print game message
	void output_msg(std::string_view msg) const;
	//print debug message
	void debug_msg(std::ostream& out, std::string_view msg) const;

private:
	//cross platform function.
	void clear_screen() const
	{
		std::print("\x1b[2J\x1b[3J\x1b[H");
		std::fflush(stdout);
	}

	void pause() const
	{
		std::print("Press Enter to continue...");
		std::string dummy;
		std::getline(std::cin, dummy);
	}
};