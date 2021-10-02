#pragma once
#include "typedefs.h"
#include "varData.h"
#include "mcString.h"
#include "endian.h"
#include <vector>

namespace Command
{
	typedef void (*Handler)(const std::vector<mcString>&);
	typedef void (*SuggestionsHandler)(const mcString&);

	namespace Parser
	{
		class Parser
		{
		public:
			virtual void write(char*&) = 0;
			virtual ~Parser();
		};

		namespace brigadier
		{
			template <class T> class Properties
			{
				static const Byte flags = 0b00;

			public:
				virtual void write(char*&);
			};
			template <class T> class PropertiesMin : public Properties<T>
			{
				static const Byte flags = 0b01;

			protected:
				T min;

			public:
				PropertiesMin(T min);
				virtual void write(char*&);
			};
			template <class T> class PropertiesMax : public Properties<T>
			{
				static const Byte flags = 0b10;

			protected:
				T max;

			public:
				PropertiesMax(T max);
				virtual void write(char*&);
			};
			template <class T> class PropertiesMinMax : public PropertiesMin<T>, public PropertiesMax<T>
			{
				static const Byte flags = 0b11;

			public:
				PropertiesMinMax(T min, T max);
				virtual void write(char*&);
			};
			class PropertiesString
			{
			public:
				static enum Mode
				{
					single_word,
					quotable_phrase,
					greedy_phrase
				};
			};

			class Boolean : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				void write(char*&);
			};
			class Double : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bdouble>* properties;

			public:
				Double(Properties<bdouble>* properties);
				~Double();
				void write(char*&);
			};
			class Float : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bfloat>* properties;

			public:
				Float(Properties<bfloat>* properties);
				~Float();
				void write(char*&);
			};
			class Integer : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bint>* properties;

			public:
				Integer(Properties<bint>* properties);
				~Integer();
				void write(char*&);
			};
			class Long : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<blong>* properties;

			public:

				Long(Properties<blong>* properties);
				~Long();
				void write(char*&);
			};
			class String : public Parser
			{
				static const mcString protocolIdentifier;
				varInt mode;

			public:
				String(PropertiesString::Mode mode);
				void write(char*&);
			};
		}

		namespace minecraft
		{
			class entity : public Parser
			{
				static const mcString protocolIdentifier;
				//single target only - 0x01
				//players only - 0x02
				Byte flags;
			public:

				entity(bool singleTargetOnly, bool playersOnly);
				virtual void write(char*&);
			};
			class game_profile : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class block_pos : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class column_pos : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class vec3 : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class vec2 : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class block_state : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class block_predicate : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class item_stack : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class item_predicate : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class color : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class component : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class message : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class nbt : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class nbt_path : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class objective : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class ovjective_criteria : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class operation : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class particle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class rotation : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class angle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class scoreboard_slot : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class score_holder : public Parser
			{
				static const mcString protocolIdentifier;
				bool allowMultiple;

			public:

				score_holder(bool allowsMultiple);
				virtual void write(char*&);
			};
			class swizzle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class team : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class item_slot : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class resource_location : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class mob_effect : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class function : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class entity_anchor : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class range : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class int_range : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class float_range : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class item_enchantment : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class entity_summon : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class dimension : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class uuid : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class nbt_tag : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class nbt_compound_tag : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
			class time : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				virtual void write(char*&);
			};
		}

		/*class minecraft
		{
		public:
			static const mcString entity,
				game_profile,
				block_pos,
				column_pos,
				vec3,
				vec2,
				block_state,
				block_predicate,
				item_stack,
				item_predicate,
				color,
				component,
				message,
				nbt,
				nbt_path,
				objective,
				ovjective_criteria,
				operation,
				particle,
				rotation,
				angle,
				scoreboard_slot,
				score_holder,
				swizzle,
				team,
				item_slot,
				resource_location,
				mob_effect,
				function,
				entity_anchor,
				range,
				int_range,
				float_range,
				item_enchantment,
				entity_summon,
				dimension,
				uuid,
				nbt_tag,
				nbt_compound_tag,
				time;
		};*/
	}

	class Node
	{
	public:

		varInt childrenCount;
		varInt* children;

		Node(varInt childrenCount, varInt* children);
		virtual ~Node();
		virtual void write(char*&) = 0;
	};
	class RootNode : public Node
	{
		static const Byte rootType = 0;
	public:

		RootNode(varInt childrenCount, varInt* children);
		virtual void write(char*&);
	};
	class LiteralNode : public Node
	{
		static const Byte literalType = 1;
	public:

		bool hasRedirect;
		varInt redirectNode;
		Handler handler;
		mcString name;

		virtual void write(char*&);
		//virtual bool match(const mcString&);
	};
	class ArgumentNode : public LiteralNode
	{
		static const Byte argumentType = 2;
	public:

		mcString* parser;
		SuggestionsHandler suggestionsHandler;

		virtual void write(char*&);
		//virtual bool match(const mcString&);
	};
}

class Node
{
public:
	//Byte flags
	enum NodeType : Byte
	{
		root,
		literal,
		argument
	} type;
	bool isExecutable;
	bool hasRedirect;
	bool hasSuggestionsType;
	SERVER_API Byte flags();

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
			enum Flags : Byte
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
			enum Flags : Byte
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
			enum Flags : Byte
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
			enum Flags : Byte
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
			enum Flags : Byte
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
			enum Flags : Byte
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
			enum Flags : Byte
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