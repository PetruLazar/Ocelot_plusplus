#include "node.h"

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

byte Node::flags()
{
	return (hasSuggestionsType << 4) | (hasRedirect << 3) | (isExecutable << 2) | type;
}

varInt Node::defaultCommandsCount = 8;
Node* Node::defaultCommands = new Node[8]{
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