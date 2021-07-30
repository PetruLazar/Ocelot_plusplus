#pragma once
#include "basic.h"
#include "varData.h"
#include "mcString.h"
#include "player.h"
#include "endian.h"

struct message
{
	struct handshake
	{
		enum class id : int
		{
			standard = 0x00,
			legacy = 0xfe
		};

		struct receive
		{
			static void standard(Player*, varInt protocolVersion, mcString& serverAdress, Port port, varInt nextState);
			//no length prefix
			//id is 'byte'
			static void legacy(Player*, byte payload);
		};
	};
	struct status
	{
		enum class id : int
		{
			response = 0x00,
			pong = 0x01,

			request = 0x00,
			ping = 0x01,
		};

		struct send
		{
			static void respose(Player*, mcString jsonResponse);
			static void pong(Player*, blong);
		};
		struct receive
		{
			static void request(Player*);
			static void ping(Player*, blong);
		};
	};
	struct login
	{
		enum class id : int
		{
			disconnect = 0x00,
			encryptionRequest = 0x01,
			success = 0x02,
			setCompression = 0x03,
			loginPluginRequest = 0x04,

			start = 0x00,
			encryptionResponse = 0x01,
			loginPluginResponse = 0x02
		};

		struct send
		{
			static void disconnect(Player*, mcString reason);
			static void encryptionRequest(Player*, varInt publicKeyLength, byte* publicKey, varInt verifyTokenLength, byte* verifyToken);
			//static void success(Player*, UUID uuid, mcString username); - UUID not implemented
			static void setCompression(Player*, varInt threshold);
			//static void loginPluginRequest(Player*, ...); - Identifier not implemented
		};
		struct receive
		{
			static void start(Player*, mcString username);
			static void encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken);
			//static void loginPluginResponse(Player*, varInt messageId, bool successful, byte* data);
		};
	};
	struct play
	{
		enum class id
		{

		};

		struct send
		{

		};
		struct receive
		{

		};
	};

	static void dispatch(Player*, char*, size_t);
};