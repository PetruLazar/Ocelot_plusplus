#pragma once
#include "typedefs.h"
#include "varData.h"
#include "mcString.h"
#include "endian.h"
#include "../player/player.h"
#include <vector>

namespace Command
{
	typedef void (*Handler)(Player* executingPlayer, const std::vector<mcString>& argumentStack);
	//typedef void (*SuggestionsHandler)(const mcString&);

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
				SERVER_API virtual void write(char*&);
			};
			template <class T> class PropertiesMin : public Properties<T>
			{
				static const Byte flags = 0b01;

			protected:
				T min;

			public:
				SERVER_API PropertiesMin(T min);
				SERVER_API virtual void write(char*&);
			};
			template <class T> class PropertiesMax : public Properties<T>
			{
				static const Byte flags = 0b10;

			protected:
				T max;

			public:
				SERVER_API PropertiesMax(T max);
				SERVER_API virtual void write(char*&);
			};
			template <class T> class PropertiesMinMax : public PropertiesMin<T>, public PropertiesMax<T>
			{
				static const Byte flags = 0b11;

			public:
				SERVER_API PropertiesMinMax(T min, T max);
				SERVER_API virtual void write(char*&);
			};
			class PropertiesString
			{
			public:
				enum Mode
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

				SERVER_API void write(char*&);
			};
			class Double : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bdouble>* properties;

			public:
				SERVER_API Double(Properties<bdouble>* properties);
				SERVER_API ~Double();
				SERVER_API void write(char*&);
			};
			class Float : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bfloat>* properties;

			public:
				SERVER_API Float(Properties<bfloat>* properties);
				SERVER_API ~Float();
				SERVER_API void write(char*&);
			};
			class Integer : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<bint>* properties;

			public:
				SERVER_API Integer(Properties<bint>* properties);
				SERVER_API ~Integer();
				SERVER_API void write(char*&);
			};
			class Long : public Parser
			{
				static const mcString protocolIdentifier;
				Properties<blong>* properties;

			public:

				SERVER_API Long(Properties<blong>* properties);
				SERVER_API ~Long();
				SERVER_API void write(char*&);
			};
			class String : public Parser
			{
				static const mcString protocolIdentifier;
				varInt mode;

			public:
				SERVER_API String(PropertiesString::Mode mode);
				SERVER_API void write(char*&);
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
				SERVER_API entity(bool singleTargetOnly, bool playersOnly);
				SERVER_API virtual void write(char*&);
			};
			class game_profile : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class block_pos : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class column_pos : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class vec3 : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class vec2 : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class block_state : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class block_predicate : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class item_stack : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class item_predicate : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class color : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class component : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class message : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class nbt : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class nbt_path : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class objective : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class objective_criteria : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class operation : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class particle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class rotation : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class angle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class scoreboard_slot : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class score_holder : public Parser
			{
				static const mcString protocolIdentifier;
				bool allowsMultiple;

			public:
				SERVER_API score_holder(bool allowsMultiple);
				SERVER_API virtual void write(char*&);
			};
			class swizzle : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class team : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class item_slot : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class resource_location : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class mob_effect : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class function : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class entity_anchor : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class range : public Parser
			{
				static const mcString protocolIdentifier;
				bool allowsDecimals;

			public:
				SERVER_API range(bool allowsDecimals);
				SERVER_API virtual void write(char*&);
			};
			class int_range : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class float_range : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class item_enchantment : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class entity_summon : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class dimension : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class uuid : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class nbt_tag : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class nbt_compound_tag : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
			class time : public Parser
			{
				static const mcString protocolIdentifier;
			public:

				SERVER_API virtual void write(char*&);
			};
		}
	}
	namespace Suggestions
	{
		class Suggestions
		{
		public:
			SERVER_API virtual void write(char*&) = 0;
		};

		namespace minecraft
		{
			class ask_server : public Suggestions
			{
				static const mcString protocolIdentifier;

			public:
				SERVER_API void write(char*&);
			};
			class all_recipes : public Suggestions
			{
				static const mcString protocolIdentifier;

			public:
				SERVER_API void write(char*&);
			};
			class available_sounds : public Suggestions
			{
				static const mcString protocolIdentifier;

			public:
				SERVER_API void write(char*&);
			};
			class available_biomes : public Suggestions
			{
				static const mcString protocolIdentifier;

			public:
				SERVER_API void write(char*&);
			};
			class summonable_entities : public Suggestions
			{
				static const mcString protocolIdentifier;

			public:
				SERVER_API void write(char*&);
			};
		}
	}

	class Node
	{
	public:

		std::vector<varInt> children;

		SERVER_API Node(std::vector<varInt> children);
		SERVER_API void addChild(varInt);
		SERVER_API virtual ~Node();
		SERVER_API virtual void write(char*&) const = 0;
	};
	class RootNode : public Node
	{
		static const Byte rootType = 0;
	public:

		SERVER_API RootNode(std::vector<varInt> children);
		SERVER_API virtual void write(char*&) const;
	};
	class LiteralNode : public Node
	{
		static const Byte literalType = 1;
	public:

		bool hasRedirect;
		varInt redirectNode;
		Handler handler;
		mcString name;

		SERVER_API LiteralNode(const mcString& name, std::vector<varInt> children, Handler handler = nullptr);
		SERVER_API LiteralNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Handler handler = nullptr);
		SERVER_API virtual ~LiteralNode();
		SERVER_API virtual void write(char*&) const;
		//virtual bool match(const mcString&);
	};
	class ArgumentNode : public LiteralNode
	{
		static const Byte argumentType = 2;
	public:

		Parser::Parser* parser;
		Suggestions::Suggestions* suggestions;

		SERVER_API ArgumentNode(const mcString& name, std::vector<varInt> children, Parser::Parser* parser, Suggestions::Suggestions* suggestions);
		SERVER_API ArgumentNode(const mcString& name, std::vector<varInt> children, Parser::Parser* parser, Handler handler = nullptr, Suggestions::Suggestions* suggestions = nullptr);
		SERVER_API ArgumentNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Parser::Parser* parser, Suggestions::Suggestions* suggestions);
		SERVER_API ArgumentNode(const mcString& name, std::vector<varInt> children, varInt redirectNode, Parser::Parser* parser, Handler handler = nullptr, Suggestions::Suggestions* suggestions = nullptr);
		SERVER_API ~ArgumentNode();
		SERVER_API virtual void write(char*&) const;
		//virtual bool match(const mcString&);
	};

	class Commands
	{
	public:
		static std::vector<Node*> commands;
		static RootNode root;

		static Commands atuomatic;
		~Commands();

		//write the commands to a buffer for sending over the network
		void write(char*&);
		//does NOT include the root node
		ull size();
	};
}