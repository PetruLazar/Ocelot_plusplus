#include "node.h"
#include "../player/command.h"

#undef min
#undef max

namespace Commands
{
	namespace Parser
	{
		Parser::~Parser() {}

		namespace brigadier
		{
			const mcString Boolean::protocolIdentifier = "brigadier:boolean",
				Double::protocolIdentifier = "brigadier:double",
				Float::protocolIdentifier = "brigadier:float",
				Integer::protocolIdentifier = "brigadier:integer",
				Long::protocolIdentifier = "brigadier:long",
				String::protocolIdentifier = "brigadier:string";

			template <class T> PropertiesMin<T>::PropertiesMin(T min) : min(min) {}
			template <class T> PropertiesMax<T>::PropertiesMax(T max) : max(max) {}
			template <class T> PropertiesMinMax<T>::PropertiesMinMax(T min, T max) : PropertiesMin<T>(min), PropertiesMax<T>(max) {}

			template <class T> void Properties<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
			}
			template <class T> void Properties<T>::limit(T value) {}
			template <class T> void PropertiesMin<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				min.write(buffer);
			}
			template <class T> void PropertiesMin<T>::limit(T value)
			{
				if (value < min) throw '\0';
			}
			template <class T> void PropertiesMax<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				max.write(buffer);
			}
			template <class T> void PropertiesMax<T>::limit(T value)
			{
				if (value > max) throw '\0';
			}
			template <class T> void PropertiesMinMax<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				PropertiesMin<T>::min.write(buffer);
				PropertiesMax<T>::max.write(buffer);
			}
			template <class T> void PropertiesMinMax<T>::limit(T value)
			{
				if (value < PropertiesMin<T>::min) throw '\0';
				if (value > PropertiesMin<T>::max) throw '\0';
			}

			void Boolean::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool Boolean::extract(mcString& command, ArgumentStack& argumentStack)
			{
				switch (command[0])
				{
				case 't':
					//check if true
					for (ull i = 1; i < 4; i++) if (command[i] != "true"[i]) return false;
					if (command[4] != ' ' && command[4]) return false;
					command.erase(0, 5);
					argumentStack.push_back(new bool(true));
					return true;
				case 'f':
					//check if false
					for (ull i = 1; i < 5; i++) if (command[i] != "false"[i]) return false;
					if (command[5] != ' ' && command[5]) return false;
					command.erase(0, 6);
					argumentStack.push_back(new bool(false));
					return true;
				}
				return false;
			}

			Double::Double(Properties<bdouble>* props) : properties(props) {}
			Double::~Double()
			{
				delete properties;
			}
			void Double::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}
			bool Double::extract(mcString& command, ArgumentStack& argumentStack)
			{
				ull after;
				double* argValue = new double;
				try
				{
					*argValue = std::stod(command, &after);
					if (command[after] != ' ' && command[after]) throw 0;
					//check for limits
					properties->limit(*argValue);
					//value is valid and within limits
					command.erase(0, after + 1);
					argumentStack.push_back(argValue);
					return true;
				}
				catch (...) {}
				delete argValue;
				return false;
			}

			Float::Float(Properties<bfloat>* props) : properties(props) {}
			Float::~Float()
			{
				delete properties;
			}
			void Float::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}
			bool Float::extract(mcString& command, ArgumentStack& argumentStack)
			{
				ull after;
				float* argValue = new float;
				try
				{
					*argValue = std::stof(command, &after);
					if (command[after] != ' ' && command[after]) 0;
					//check for limits
					properties->limit(*argValue);
					//value is valid and within limits
					command.erase(0, after + 1);
					argumentStack.push_back(argValue);
					return true;
				}
				catch (...) {}
				delete argValue;
				return false;
			}

			Integer::Integer(Properties<bint>* props) : properties(props) {}
			Integer::~Integer()
			{
				delete properties;
			}
			void Integer::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}
			bool Integer::extract(mcString& command, ArgumentStack& argumentStack)
			{
				ull after;
				int* argValue = new int;
				try
				{
					*argValue = std::stoi(command, &after);
					if (command[after] != ' ' && command[after]) throw 0;
					//check for limits
					properties->limit(*argValue);
					//value is valid and within limits
					command.erase(0, after + 1);
					argumentStack.push_back(argValue);
					return true;
				}
				catch (...) {}
				delete argValue;
				return false;
			}

			Long::Long(Properties<blong>* props) : properties(props) {}
			Long::~Long()
			{
				delete properties;
			}
			void Long::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}
			bool Long::extract(mcString& command, ArgumentStack& argumentStack)
			{
				ull after;
				int64* argValue = new int64;
				try
				{
					*argValue = std::stoll(command, &after);
					if (command[after] != ' ' && command[after]) throw 0;
					//check for limits
					properties->limit(*argValue);
					//value is valid and within limits
					command.erase(0, after + 1);
					argumentStack.push_back(argValue);
					return true;
				}
				catch (...) {}
				delete argValue;
				return false;
			}

			String::String(PropertiesString::Mode mode) : mode(mode) {}
			void String::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				mode.write(buffer);
			}
			bool String::extract(mcString& command, ArgumentStack& argumentStack)
			{
				switch (mode)
				{
				case PropertiesString::single_word:
				{
					ull argLen = command.find(' ');
					if (argLen == -1) argLen = command.length();
					argumentStack.push_back(new mcString(command, 0, argLen));
					command.erase(0, argLen + 1);
					return true;
				}
				case PropertiesString::quotable_phrase:
					if (command[0] != '"') return false;
					for (ull i = 1;; i++)
					{
						switch (command[i])
						{
						case 0:
							return false;
						case '"':
							argumentStack.push_back(new mcString(command, 1, i - 2));
							command.erase(0, i + 2);
							return true;
						case '\\':
							i++;
						}
					}
				case PropertiesString::greedy_phrase:
					argumentStack.push_back(new mcString(command));
					command.clear();
					return true;
				}
				return false;
			}
		}

		namespace minecraft
		{
			const mcString entity::protocolIdentifier = "minecraft:entity",
				game_profile::protocolIdentifier = "minecraft:game_profile",
				block_pos::protocolIdentifier = "minecraft:block_pos",
				column_pos::protocolIdentifier = "minecraft:column_pos",
				vec3::protocolIdentifier = "minecraft:vec3",
				vec2::protocolIdentifier = "minecraft:vec2",
				block_state::protocolIdentifier = "minecraft:block_state",
				block_predicate::protocolIdentifier = "minecraft:block_predicate",
				item_stack::protocolIdentifier = "minecraft:item_stack",
				item_predicate::protocolIdentifier = "minecraft:item_predicate",
				color::protocolIdentifier = "minecraft:color",
				component::protocolIdentifier = "minecraft:component",
				message::protocolIdentifier = "minecraft:message",
				nbt::protocolIdentifier = "minecraft:nbt",
				nbt_path::protocolIdentifier = "minecraft:nbt_path",
				objective::protocolIdentifier = "minecraft:objective",
				objective_criteria::protocolIdentifier = "minecraft:objective_criteria",
				operation::protocolIdentifier = "minecraft:operation",
				particle::protocolIdentifier = "minecraft:particle",
				rotation::protocolIdentifier = "minecraft:rotation",
				angle::protocolIdentifier = "minecraft:angle",
				scoreboard_slot::protocolIdentifier = "minecraft:scoreboard_slot",
				score_holder::protocolIdentifier = "minecraft:score_holder",
				swizzle::protocolIdentifier = "minecraft:swizzle",
				team::protocolIdentifier = "minecraft:team",
				item_slot::protocolIdentifier = "minecraft:item_slot",
				resource_location::protocolIdentifier = "minecraft:resource_location",
				mob_effect::protocolIdentifier = "minecraft:mob_effect",
				function::protocolIdentifier = "minecraft:function",
				entity_anchor::protocolIdentifier = "minecraft:entity_anchor",
				range::protocolIdentifier = "minecraft:range",
				int_range::protocolIdentifier = "minecraft:int_range",
				float_range::protocolIdentifier = "minecraft:float_range",
				item_enchantment::protocolIdentifier = "minecraft:item_enchantment",
				entity_summon::protocolIdentifier = "minecraft:entity_summon",
				dimension::protocolIdentifier = "minecraft:dimension",
				uuid::protocolIdentifier = "minecraft:uuid",
				nbt_tag::protocolIdentifier = "minecraft:nbt_tag",
				nbt_compound_tag::protocolIdentifier = "minecraft:nbt_compound_tag",
				time::protocolIdentifier = "minecraft:time";

			entity::entity(bool singleTargetsOnly, bool playersOnly) : flags((playersOnly << 1) | (int)singleTargetsOnly) {}
			void entity::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = flags;
			}
			bool entity::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void game_profile::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool game_profile::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void block_pos::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool block_pos::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void column_pos::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool column_pos::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void vec3::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool vec3::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void vec2::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool vec2::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void block_state::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool block_state::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void block_predicate::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool block_predicate::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void item_stack::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool item_stack::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void item_predicate::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool item_predicate::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void color::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool color::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void component::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool component::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void message::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool message::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void nbt::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool nbt::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void nbt_path::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool nbt_path::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void objective::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool objective::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void objective_criteria::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool objective_criteria::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void particle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool particle::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void rotation::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool rotation::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void angle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool angle::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void scoreboard_slot::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool scoreboard_slot::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			score_holder::score_holder(bool allowsMultiple) : allowsMultiple(allowsMultiple) {}
			void score_holder::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = allowsMultiple;
			}
			bool score_holder::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void swizzle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool swizzle::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void team::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool team::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void item_slot::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool item_slot::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void resource_location::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool resource_location::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void mob_effect::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool mob_effect::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void function::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool function::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void entity_anchor::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool entity_anchor::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			range::range(bool allowsDecimals) : allowsDecimals(allowsDecimals) {}
			void range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = allowsDecimals;
			}
			bool range::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void int_range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool int_range::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void float_range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool float_range::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void item_enchantment::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool item_enchantment::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void entity_summon::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool entity_summon::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void dimension::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool dimension::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void uuid::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool uuid::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void nbt_tag::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool nbt_tag::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void nbt_compound_tag::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool nbt_compound_tag::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
			void time::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			bool time::extract(mcString& command, ArgumentStack& argumentStack)
			{
				throw "WIP";
			}
		}
	}
	namespace Suggestions::minecraft
	{
		const mcString ask_server::protocolIdentifier = "minecraft:ask_server",
			all_recipes::protocolIdentifier = "minecraft:all_recipes",
			available_sounds::protocolIdentifier = "minecraft:available_sounds",
			available_biomes::protocolIdentifier = "minecraft:available_biomes",
			summonable_entities::protocolIdentifier = "minecraft:summonable_entities";

		void ask_server::write(char*& buffer)
		{
			protocolIdentifier.write(buffer);
		}
		void all_recipes::write(char*& buffer)
		{
			protocolIdentifier.write(buffer);
		}
		void available_sounds::write(char*& buffer)
		{
			protocolIdentifier.write(buffer);
		}
		void available_biomes::write(char*& buffer)
		{
			protocolIdentifier.write(buffer);
		}
		void summonable_entities::write(char*& buffer)
		{
			protocolIdentifier.write(buffer);
		}
	}

	//flags
			//node type: 0b00011 -0x03
				//root - 0
				//literal - 1
				//argument - 2
			//executable - 0b00100 - 0x04
			//redirects - 0b01000 - 0x08
			//hasSuggestions - 0b10000 - 0x10

	//childrenCount [varInt]
	//children (index) [varInt]
	//redirect node (index) [varInt] - if flags - redirects
	//name [mcString] - for argument and literal nodes
	//parser [mcString] - for argument nodes
	//properties [varies] - for argument nodes

	//suggestions [mcString] - if flags - hasSuggestions

	Node::Node(std::vector<varInt> children) : children(children) {}
	void Node::addChild(varInt c)
	{
		children.push_back(c);
	}
	Node::~Node() {}
	void Node::execute(Player* executingPlayer, ArgumentStack& argumentStack)
	{
		throw Chat("Unknown command", Chat::color::red());
	}

	RootNode::RootNode(std::vector<varInt> children) : Node(children) {}
	void RootNode::write(char*& buffer) const
	{
		*(buffer++) = rootType;
		varInt((int)children.size()).write(buffer);
		for (varInt child : children) child.write(buffer);
		//no redirect
		//no name
		//no parser
		//no properties
		//no suggestions
	}
	bool RootNode::extract(mcString&, ArgumentStack&)
	{
		throw std::exception("root node not parsable");
	}

	LiteralNode::LiteralNode(const mcString& name, std::vector<varInt> children, Handler handler) : Node(children), name(name), handler(handler), hasRedirect(false) {}
	LiteralNode::LiteralNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Handler handler) : Node(children), name(name), hasRedirect(true), redirectNode(redirectNode), handler(handler) {}
	LiteralNode::~LiteralNode() {}
	void LiteralNode::write(char*& buffer) const
	{
		*(buffer++) = (hasRedirect << 3) | ((handler != 0) << 2) | literalType;
		varInt((int)children.size()).write(buffer);
		for (varInt child : children) child.write(buffer);
		if (hasRedirect) redirectNode.write(buffer);
		name.write(buffer);
		//no parser
		//no properties
		//no suggestions
	}
	bool LiteralNode::extract(mcString& command, ArgumentStack&)
	{
		ull nameSize = name.length();
		if (command.length() < nameSize || (command[nameSize] != ' ' && command[nameSize])) return false;

		for (ull i = 0; i < nameSize; i++) if (name[i] != command[i]) return false;

		command.erase(0, nameSize + 1);
		return true;
	}
	void LiteralNode::execute(Player* executingPlayer, ArgumentStack& argumentStack)
	{
		if (!handler) throw Chat("Command not executable", Chat::color::red());
		handler(executingPlayer, argumentStack);
	}

	ArgumentNode::ArgumentNode(const mcString& name, std::vector<varInt> children, Parser::Parser* parser, Suggestions::Suggestions* suggestions) : LiteralNode(name, children), parser(parser), suggestions(suggestions) {}
	ArgumentNode::ArgumentNode(const mcString& name, std::vector<varInt> children, Parser::Parser* parser, Handler handler, Suggestions::Suggestions* suggestions) : LiteralNode(name, children, handler), parser(parser), suggestions(suggestions) {}
	ArgumentNode::ArgumentNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Parser::Parser* parser, Suggestions::Suggestions* suggestions) : LiteralNode(name, children, redirectNode), parser(parser), suggestions(suggestions) {}
	ArgumentNode::ArgumentNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Parser::Parser* parser, Handler handler, Suggestions::Suggestions* suggestions) : LiteralNode(name, children, redirectNode, handler), parser(parser), suggestions(suggestions) {}
	ArgumentNode::~ArgumentNode()
	{
		delete parser;
		if (suggestions) delete suggestions;
	}
	void ArgumentNode::write(char*& buffer) const
	{
		*(buffer++) = ((suggestions != 0) << 4) | (hasRedirect << 3) | ((handler != 0) << 2) | argumentType;
		varInt((int)children.size()).write(buffer);
		for (varInt child : children) child.write(buffer);
		if (hasRedirect) redirectNode.write(buffer);
		name.write(buffer);
		parser->write(buffer); // includes properties
		if (suggestions) suggestions->write(buffer);
	}
	bool ArgumentNode::extract(mcString& command, ArgumentStack& argumentStack)
	{
		return parser->extract(command, argumentStack);
	}

	using namespace Parser;
	using namespace brigadier;

	std::vector<Node*> Commands::commands{
		/*  0 */ new LiteralNode("survival", { }, CommandHandlers::gamemodeSurvival),
		/*  1 */ new LiteralNode("creative", { }, CommandHandlers::gamemodeCreative),
		/*  2 */ new LiteralNode("adventure", { }, CommandHandlers::gamemodeAdventure),
		/*  3 */ new LiteralNode("spectator", { }, CommandHandlers::gamemodeSpectator),
		/*  4 */ new LiteralNode("gamemode", { 0, 1, 2, 3 }),
		/*  5 */ new ArgumentNode("world name", { }, new String(PropertiesString::single_word), CommandHandlers::worldChange, new Suggestions::minecraft::ask_server()),
		/*  6 */ new LiteralNode("world", { 5 }, CommandHandlers::tellWorld),
		/*  7 */ new LiteralNode("ping", { }, CommandHandlers::tellPing),
		/*  8 */ new ArgumentNode("test arguments", { }, new String(PropertiesString::greedy_phrase), CommandHandlers::test),
		/*  9 */ new ArgumentNode("test number", { 8 }, new Integer(new PropertiesMin<bint>(1)), CommandHandlers::test),
		/* 10 */ new LiteralNode("test", { 9 }),
		/* 11 */ new ArgumentNode("tp z", { }, new Double(new Properties<bdouble>()), CommandHandlers::teleport),
		/* 12 */ new ArgumentNode("tp y", { 11 }, new Double(new Properties<bdouble>())),
		/* 13 */ new ArgumentNode("tp x", { 12 }, new Double(new Properties<bdouble>())),
		/* 14 */ new ArgumentNode("destination player", { }, new String(PropertiesString::single_word), CommandHandlers::teleport),
		/* 15 */ new ArgumentNode("player", { 13, 14}, new String(PropertiesString::single_word), CommandHandlers::teleport),
		/* 16 */ new LiteralNode("tp", { 13, 15 }),
		/* 17 */ new ArgumentNode("spawn z", { }, new Double(new Properties<bdouble>()),CommandHandlers::setSpawn),
		/* 18 */ new ArgumentNode("spawn y", { 17 }, new Double(new Properties<bdouble>())),
		/* 19 */ new ArgumentNode("spawn x", { 18 }, new Double(new Properties<bdouble>())),
		/* 20 */ new LiteralNode("spawn", { 19 }, CommandHandlers::spawn),
		/* 21 */ new LiteralNode("stop", { }, CommandHandlers::stopServer),
		/* 22 */ new LiteralNode("restart", { }, CommandHandlers::restartServer),
		/* 23 */ new ArgumentNode("tick speed", { }, new Integer(new PropertiesMin<bint>(0)), CommandHandlers::randomTickSpeed),
		/* 24 */ new LiteralNode("randomTickSpeed", { 23 }, CommandHandlers::randomTickSpeed)
	};
	RootNode Commands::root = std::vector<varInt>{ 4, 6, 7, 10, 16, 20, 21, 22, 24 };

	Commands Commands::atuomatic;
	Commands::~Commands()
	{
		for (Node* p : commands) delete p;
	}
}