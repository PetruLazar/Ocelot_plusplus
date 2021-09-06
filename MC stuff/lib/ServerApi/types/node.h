#pragma once
#include "typedefs.h"
#include "varData.h"
#include "mcString.h"
#include "endian.h"

class Node
{
public:
	//byte flags
	enum NodeType : byte
	{
		root,
		literal,
		argument
	} type;
	bool isExecutable;
	bool hasRedirect;
	bool hasSuggestionsType;
	SERVER_API byte flags();

	//children array
	varInt childrenCount;
	varInt* children;

	//redirect node
	varInt redirectNode;

	//name
	mcString* name; // only for argument and literal nodes

	//parser
	mcString* parser; //only for argument nodes

	//properties
	union Properties
	{
		class DoubleProperties
		{
		public:
			enum Flags : byte
			{
				none = 0x00,
				hasMin = 0x01,
				hasMax = 0x02
			} flags;
			bdouble min; // if flags & 0x01
			bdouble max; // if flags & 0x02
		} doubleProperties;
		class FloatProperties
		{
		public:
			enum Flags : byte
			{
				none = 0x00,
				hasMin = 0x01,
				hasMax = 0x02
			} flags;
			bfloat min;
			bfloat max;
		} floatProperties;
		class IntegerProperties
		{
		public:
			enum Flags : byte
			{
				none = 0x00,
				hasMin = 0x01,
				hasMax = 0x02
			} flags;
			bint min;
			bint max;
		} integerProperties;
		class LongProperties
		{
		public:
			enum Flags : byte
			{
				none = 0x00,
				hasMin = 0x01,
				hasMax = 0x02
			} flags;
			blong min;
			blong max;
		} longProperties;
		class StringProperties
		{
		public:
			enum Flags : byte
			{
				SINGLE_WORD,
				QUOTABLE_PHRASE,
				GREEDY_PHRASE
			};
			varInt flag;

			StringProperties(Flags flag);
		} stringProperties;
		class EntityProperties
		{
		public:
			//0x01 - single entity/player
			//0x02 - players
			enum Flags : byte
			{
				none = 0x00,
				singleTarget = 0x01,
				playerTarget = 0x02
			} flags;
		} entityProperties;
		class ScoreHolderProperties
		{
		public:
			//0x01 - allows multiple
			enum Flags : byte
			{
				none = 0x00,
				allowsMultiple = 0x01
			} flags;
		} scoreHolderProperties;
		class RangeProperties
		{
		public:
			bool decimals;
		} rangeProperties;

		Properties(const DoubleProperties&);
		Properties(const FloatProperties&);
		Properties(const IntegerProperties&);
		Properties(const LongProperties&);
		Properties(const StringProperties&);
		Properties(const EntityProperties&);
		Properties(const ScoreHolderProperties&);
		Properties(const RangeProperties&);
		Properties();
	} properties;

	//suggestions type
	mcString* suggestionsType; //only if hasSuggestionsType is true

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::DoubleProperties& doubleProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::FloatProperties& floatProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::IntegerProperties& integerProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::LongProperties& longProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::StringProperties& stringProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::EntityProperties& Properties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::ScoreHolderProperties& scoreHolderProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, const Properties::RangeProperties& rangeProperties, mcString* suggestionsType);

	SERVER_API Node(NodeType type, bool isExecutable, bool hasRedirect, bool hasSuggestionsType,
		varInt childrenCount, varInt* children,
		varInt redirectNode, mcString* name, mcString* parser, mcString* suggestionsType);

	SERVER_API ~Node();

	SERVER_API static varInt defaultCommandsCount;
	SERVER_API static Node defaultCommands[];
	SERVER_API static varInt defaultCommandsRootIndex;
};