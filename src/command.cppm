module;
#include <tao/pegtl.hpp>

export module command;
import std;

export enum class PlayerAction
{
	move,
	shoot,
	unknown
};

export struct PlayerController
{
	PlayerAction player_action{ PlayerAction::unknown };
	int room_id{ };
	std::vector<int> room_ids;
};

export namespace grammar
{
	struct act_move : tao::pegtl::one<'m'> { };
	struct act_shoot : tao::pegtl::one<'s'> { };
	struct sign : tao::pegtl::one<'-'> { };
	struct single_room_id : tao::pegtl::plus<tao::pegtl::range<'0', '9'>> { };
	struct room_id : tao::pegtl::plus<tao::pegtl::range<'0', '9'>> { };

	struct room_id_list : tao::pegtl::seq<room_id, tao::pegtl::rep_min_max<0, 2, sign, room_id>> { };

	struct cmd_move : tao::pegtl::seq<act_move, single_room_id> { };
	struct cmd_shoot : tao::pegtl::seq<act_shoot, room_id_list> { };

	struct command : tao::pegtl::sor<cmd_move, cmd_shoot> { };
	struct root : tao::pegtl::seq<command, tao::pegtl::eof> { };
}


export namespace command_action
{
	template<typename Rule>
	struct action : tao::pegtl::nothing<Rule> { };

	template<>
	struct action<grammar::single_room_id>
	{
		template<typename Input>
		static void apply(const Input& in, PlayerController& state)
		{
			const std::string s = in.string();
			int id{ };
			const auto result = std::from_chars(s.data(), s.data() + s.size(), id);
			if (result.ec == std::errc{ })
			{
				state.room_id = id;
			}
		}
	};

	template<>
	struct action<grammar::room_id>
	{
		template<typename Input>
		static void apply(const Input& in, PlayerController& state)
		{
			const std::string s = in.string();
			int id{ };
			const auto result = std::from_chars(s.data(), s.data() + s.size(), id);
			if (result.ec == std::errc{ })
			{
				state.room_ids.push_back(id);
			}
		}
	};

	template<>
	struct action<grammar::act_move>
	{
		template<typename Input>
		static void apply(const Input&, PlayerController& state)
		{
			state.player_action = PlayerAction::move;
		}
	};

	template<>
	struct action<grammar::act_shoot>
	{
		template<typename Input>
		static void apply(const Input&, PlayerController& state)
		{
			state.player_action = PlayerAction::shoot;
		}
	};
}

export bool parse_command(std::string_view cmd, PlayerController& st)
{
	tao::pegtl::base_input in(cmd);
	return tao::pegtl::parse<grammar::root, command_action::action>(in, st);
}