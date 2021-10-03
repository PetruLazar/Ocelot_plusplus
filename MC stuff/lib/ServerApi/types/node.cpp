#include "node.h"

namespace Command
{
	namespace Parser
	{
		Parser::~Parser() { }

		namespace brigadier
		{
			const mcString Boolean::protocolIdentifier = "brigadier:boolean",
				Double::protocolIdentifier = "brigadier:double",
				Float::protocolIdentifier = "brigadier:float",
				Integer::protocolIdentifier = "brigadier:integer",
				Long::protocolIdentifier = "brigadier:long",
				String::protocolIdentifier = "brigadier:string";

			template <class T> PropertiesMin<T>::PropertiesMin(T min) : min(min) { }
			template <class T> PropertiesMax<T>::PropertiesMax(T max) : max(max) { }
			template <class T> PropertiesMinMax<T>::PropertiesMinMax(T min, T max) : PropertiesMin<T>(min), PropertiesMax<T>(max) { }

			template <class T> void Properties<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
			}
			template <class T> void PropertiesMin<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				min.write(buffer);
			}
			template <class T> void PropertiesMax<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				max.write(buffer);
			}
			template <class T> void PropertiesMinMax<T>::write(char*& buffer)
			{
				*(buffer++) = flags;
				PropertiesMin<T>::min.write(buffer);
				PropertiesMax<T>::max.write(buffer);
			}

			void Boolean::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}

			Double::Double(Properties<bdouble>* props) : properties(props) { }
			Double::~Double()
			{
				delete properties;
			}
			void Double::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}

			Float::Float(Properties<bfloat>* props) : properties(props) { }
			Float::~Float()
			{
				delete properties;
			}
			void Float::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}

			Integer::Integer(Properties<bint>* props) : properties(props) { }
			Integer::~Integer()
			{
				delete properties;
			}
			void Integer::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}

			Long::Long(Properties<blong>* props) : properties(props) { }
			Long::~Long()
			{
				delete properties;
			}
			void Long::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				properties->write(buffer);
			}

			String::String(PropertiesString::Mode mode) : mode(mode) { }
			void String::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				mode.write(buffer);
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

			entity::entity(bool singleTargetsOnly, bool playersOnly) : flags((playersOnly << 1) | singleTargetsOnly) { }
			void entity::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = flags;
			}
			void game_profile::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void block_pos::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void column_pos::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void vec3::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void vec2::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void block_state::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void block_predicate::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void item_stack::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void item_predicate::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void color::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void component::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void message::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void nbt::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void nbt_path::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void objective::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void objective_criteria::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void particle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void rotation::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void angle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void scoreboard_slot::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			score_holder::score_holder(bool allowsMultiple) : allowsMultiple(allowsMultiple) { }
			void score_holder::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = allowsMultiple;
			}
			void swizzle::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void team::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void item_slot::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void resource_location::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void mob_effect::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void function::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void entity_anchor::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			range::range(bool allowsDecimals) : allowsDecimals(allowsDecimals) { }
			void range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
				*(buffer++) = allowsDecimals;
			}
			void int_range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void float_range::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void item_enchantment::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void entity_summon::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void dimension::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void uuid::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void nbt_tag::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void nbt_compound_tag::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
			}
			void time::write(char*& buffer)
			{
				protocolIdentifier.write(buffer);
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

	Node::Node(varInt childrenCount, varInt* children) : childrenCount(childrenCount), children(children) { }
	Node::~Node()
	{
		delete[] children;
	}

	RootNode::RootNode(varInt childrenCount, varInt* children) : Node(childrenCount, children) { }
	void RootNode::write(char*& buffer)
	{
		*(buffer++) = rootType;
		childrenCount.write(buffer);
		for (int i = 0; i < childrenCount; i++) children[i].write(buffer);
		//no redirect
		//no name
		//no parser
		//no properties
		//no suggestions
	}

	LiteralNode::LiteralNode(const mcString& name, varInt childrenCount, varInt* children, Handler handler) : Node(childrenCount, children), name(name), handler(handler), hasRedirect(false) { }
	LiteralNode::LiteralNode(const mcString& name, varInt childrenCount, varInt* children, varInt redirectNode, Handler handler) : Node(childrenCount, children), name(name), hasRedirect(true), redirectNode(redirectNode), handler(handler) { }
	void LiteralNode::write(char*& buffer)
	{
		*(buffer++) = (hasRedirect << 3) | ((handler != 0) << 2) | literalType;
		childrenCount.write(buffer);
		for (int i = 0; i < childrenCount; i++) children[i].write(buffer);
		if (hasRedirect) redirectNode.write(buffer);
		name.write(buffer);
		//no parser
		//no properties
		//no suggestions
	}

	ArgumentNode::ArgumentNode(const mcString& name, varInt childrenCount, varInt* children, Parser::Parser* parser, Suggestions::Suggestions* suggestions) : LiteralNode(name, childrenCount, children), parser(parser), suggestions(suggestions) { }
	ArgumentNode::ArgumentNode(const mcString& name, varInt childrenCount, varInt* children, Parser::Parser* parser, Handler handler = nullptr, Suggestions::Suggestions* suggestions = nullptr) : LiteralNode(name, childrenCount, children, handler), parser(parser), suggestions(suggestions) { }
	ArgumentNode::ArgumentNode(const mcString& name, varInt childrenCount, varInt* children, varInt redirectNode, Parser::Parser* parser, Suggestions::Suggestions* suggestions) : LiteralNode(name, childrenCount, children, redirectNode), parser(parser), suggestions(suggestions) { }
	ArgumentNode::ArgumentNode(const mcString& name, varInt childrenCount, varInt* children, varInt redirectNode, Parser::Parser* parser = nullptr, Suggestions::Suggestions* suggestions = nullptr) : LiteralNode(name, childrenCount, children, redirectNode, handler), parser(parser), suggestions(suggestions) { }
	void ArgumentNode::write(char*& buffer)
	{
		*(buffer++) = ((suggestions != 0) << 4) | (hasRedirect << 3) | ((handler != 0) << 2) | argumentType;
		childrenCount.write(buffer);
		for (int i = 0; i < childrenCount; i++) children[i].write(buffer);
		if (hasRedirect) redirectNode.write(buffer);
		name.write(buffer);
		parser->write(buffer); // includes properties
		if (suggestions) suggestions->write(buffer);
	}
}

Node::Properties::Properties(const DoubleProperties& properties) : doubleProperties(properties) { }
Node::Properties::Properties(const FloatProperties& properties) : floatProperties(properties) { }
Node::Properties::Properties(const IntegerProperties& properties) : integerProperties(properties) { }
Node::Properties::Properties(const LongProperties& properties) : longProperties(properties) { }
Node::Properties::Properties(const StringProperties& properties) : stringProperties(properties) { }
Node::Properties::Properties(const EntityProperties& properties) : entityProperties(properties) { }
Node::Properties::Properties(const ScoreHolderProperties& properties) : scoreHolderProperties(properties) { }
Node::Properties::Properties(const RangeProperties& properties) : rangeProperties(properties) { }
Node::Properties::Properties() { }

Node::Properties::StringProperties::StringProperties(Node::Properties::StringProperties::Flags flag) : flag(flag) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::DoubleProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::FloatProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::IntegerProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::LongProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::StringProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::EntityProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::ScoreHolderProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, const Properties::RangeProperties& properties, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType), properties(properties) { }

Node::Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
	varInt childrenCount, varInt* children,
	varInt redirectNode, mcString* name, mcString* parser, mcString* suggestionsType) :
	type(type), isExecutable(isExecutable), hasRedirect(hasRedirect), hasSuggestionsType(hasSuggestionsType), childrenCount(childrenCount), children(children),
	redirectNode(redirectNode), name(name), parser(parser), suggestionsType(suggestionsType) { }

Node::~Node()
{
	if (children) delete[] children;
	if (name) delete name;
	if (parser) delete parser;
	if (suggestionsType) delete suggestionsType;
}

Byte Node::flags()
{
	return (hasSuggestionsType << 4) | (hasRedirect << 3) | (isExecutable << 2) | type;
}

varInt Node::defaultCommandsCount = 8;
Node Node::defaultCommands[] = {
	Node(Node::literal,true,false,false,0,nullptr,0,new mcString("creative"),nullptr,nullptr),
	Node(Node::literal,true,false,false,0,nullptr,0,new mcString("survival"),nullptr,nullptr),
	Node(Node::literal,true,false,false,0,nullptr,0,new mcString("spectator"),nullptr,nullptr),
	Node(Node::literal,true,false,false,0,nullptr,0,new mcString("adventure"),nullptr,nullptr),
	Node(Node::literal,true,false,false,4,new varInt[4]{0,1,2,3},0,new mcString("gamemode"),nullptr,nullptr),
	Node(Node::argument,true,false,true,0,nullptr,0,new mcString("world name"),new mcString("brigadier:string"),Node::Properties::StringProperties(Node::Properties::StringProperties::SINGLE_WORD),new mcString("minecraft:ask_server")),
	Node(Node::literal,true,false,false,1,new varInt[1]{5},0,new mcString("world"),nullptr,nullptr),
	Node(Node::root,false,false,false,2,new varInt[2]{4,6},0,nullptr,nullptr,nullptr)
};
varInt Node::defaultCommandsRootIndex = Node::defaultCommandsCount - 1;