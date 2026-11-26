export module vertex;

import std;

export class Vertex
{
public:

	enum class StateFlags
	{
		player_state = 0,
		wumpus_state,
		hole_state,
		bug_state
	};



	Vertex(int id = -1, std::bitset<4> state = 0)
		:m_id{ id }, m_bitset{ state }
	{ }

	explicit Vertex(int id) :Vertex{ id,0 }
	{ }

	explicit Vertex(std::bitset<4> state) :Vertex{ -1,state }
	{ }

	void set_id(int id) { m_id = id; }
	int get_id() const { return m_id; }

	void set_state(StateFlags flag) { m_bitset.set(std::to_underlying(flag)); }
	void flip_state(StateFlags flag) { m_bitset.flip(std::to_underlying(flag)); }
	void set_state(int pos) { m_bitset.set(pos); }
	std::bitset<4> get_state() const { return m_bitset; }
	void reset_state() { m_bitset.reset(); }


private:
	//room number
	int m_id{ -1 };
	//room states
	std::bitset<4> m_bitset;
};