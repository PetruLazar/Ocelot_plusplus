#include "message.h"

void message::handshake::receive::standard(Player* p, varInt protocolVersion, mcString& serverAdress, Port port, varInt nextState)
{
	p->state = (ConnectionState)(int)nextState;
}
void message::handshake::receive::legacy(Player* p, byte payload)
{
	throw "Legacy handshake not yet implemented";
}

void message::status::send::respose(Player* p, mcString jsonResponse)
{
	char* lendata = new char[4], * lendatastart = lendata,
		* data = new char[4 * 1024], * start = data;

	varInt id_ = (int)id::response;

	id_.write(data);
	jsonResponse.write(data);

	varInt len = int(data - start);
	len.write(lendata);

	p->send(lendatastart, lendata - lendatastart);
	p->send(start, data - start);

	delete[] lendatastart;
	delete[] start;
}
void message::status::send::pong(Player* p, blong payload)
{
	char* lendata = new char[4], * lendatastart = lendata,
		* data = new char[9], * start = data;

	varInt id_ = (int)id::pong, len;

	id_.write(data);
	payload.write(data);

	len = int(data - start);
	len.write(lendata);

	p->send(lendatastart, lendata - lendatastart);
	p->send(start, data - start);

	p->disconnect();

	delete[] lendatastart;
	delete[] start;
}

/*
{
	"version": {
		"name": "1.17.1",
		"protocol": 756
	},
	"players": {
		"max": 100,
		"online": 5
	},
	"description": {
		"text": "Hello world"
	}
}
*/

void message::status::receive::request(Player* p)
{
	message::status::send::respose(p, "{\"version\":{\"name\":\"1.17.1\",\"protocol\":756},\"players\":{\"max\":" + std::to_string(rand() % 20 + 20) + ",\"online\":" + std::to_string(rand() % 20) + ",\"sample\":[{\"name\":\"TheGoldenSnowman\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"},{\"name\":\"TimmyBrott\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d21\"},{\"name\":\"NativeLog05\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d22\"},{\"name\":\"Tim\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d23\"}]},\"description\":{\"text\":\"Minigames (" + std::to_string(rand() % 100) + ")\n    by \",\"extra\":[{\"text\":\"The\",\"color\":\"#aa6946\"},{\"text\":\"Golden\",\"color\":\"gold\"},{\"text\":\"Snowman\",\"color\":\"white\"}]}}");
}
void message::status::receive::ping(Player* p, blong payload)
{
	message::status::send::pong(p, payload);
}

void message::login::send::disconnect(Player* p, mcString reason)
{
	varInt id = (int)id::disconnect;
	char* lendata = new char[4], * lendatastart = lendata,
		* data = new char[1024 * 1024], * start = data;

	id.write(data);
	reason.write(data);

	varInt length = int(data - start);
	length.write(lendata);

	p->send(lendatastart, lendata - lendatastart);
	p->send(start, data - start);

	p->disconnect();

	delete[] lendatastart;
	delete[] start;
}
void message::login::send::encryptionRequest(Player*, varInt publicKeyLength, byte* publicKey, varInt verifyTokenLength, byte* verifyToken)
{
	throw "Encryption not supported";
}
void message::login::send::setCompression(Player*, varInt threshold)
{
	throw "compression not supported";
}

void message::login::receive::start(Player* p, mcString username)
{
	login::send::disconnect(p, "{\"text\":\"Fuck off, " + (std::string)username + "!\",\"color\":\"dark_red\",\"bold\":\"true\"}");
}
void message::login::receive::encryptionResponse(Player*, varInt sharedSecretLength, byte* sharedSecret, varInt verifyTokenLength, byte* verifyToken)
{
	throw "Encryption not supported";
}

void message::dispatch(Player* p, char* data, size_t size)
{
	varInt id;
	id.read(data);
	switch (p->state)
	{
	case ConnectionState::handshake:
		switch ((handshake::id)(int)id)
		{
		case handshake::id::standard:
		{
			varInt protocolV, nextState;
			mcString address;
			bigEndian<Port> port;
			protocolV.read(data);
			address.read(data);
			port.read(data);
			nextState.read(data);
			handshake::receive::standard(p, protocolV, address, port, nextState);
		}
		break;
		default:
			p->disconnect();
			throw "Invalid packet id";
		}
		break;
	case ConnectionState::status:
		switch ((status::id)(int)id)
		{
		case status::id::request:
		{
			status::receive::request(p);
		}
		break;
		case status::id::ping:
		{
			blong payload;
			payload.read(data);
			status::receive::ping(p, payload);
		}
		break;
		default:
			p->disconnect();
			throw "Invalid packet id";
		}
		break;
	case ConnectionState::login:
		switch ((login::id)(int)id)
		{
		case login::id::start:
		{
			mcString username;
			username.read(data);
			login::receive::start(p, username);
		}
		break;
		case login::id::encryptionResponse:
		{
			login::receive::encryptionResponse(p, 0, nullptr, 0, nullptr);
		}
		break;
		case login::id::loginPluginResponse:
		{
			p->disconnect();
			throw "Login Plugin not supported.";
		}
		break;
		default:
			p->disconnect();
			throw "Invalid packet id";
		}
		break;
	default:
		p->disconnect();
		throw "Invalid connection state";
	}
}