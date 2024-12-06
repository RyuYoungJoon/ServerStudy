#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);


	return true;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation 체크
	Protocol::S_LOGIN loginPacket;
	loginPacket.set_success(true);

	// DB에서 플레이어 정보 긁어온다.
	// GameSession에 플레이어 정보를 저장(메모리)

	// ID 발급 (DB 아이디가 아니고, 인게임 아이디)
	static Atomic<uint64> idGenerator = 1;

	{
		auto player = loginPacket.add_players();
		player->set_name(u8"DBasddf1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);
		player->set_id(idGenerator++);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = player->id();
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		
		gameSession->_players.push_back(playerRef);
		
	}
	{
		auto player = loginPacket.add_players();
		player->set_name(u8"DBasd2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);
		player->set_id(idGenerator++);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	// TODO : Validation
	
	PlayerRef player = gameSession->_players[index]; // READ ONLY?

	GRoom.Enter(player);	// WRITE_LOCK

	Protocol::S_ENTER_GAME enterGamePacket;
	enterGamePacket.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
	player->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPacket;
	chatPacket.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPacket);

	GRoom.Broadcast(sendBuffer); // WRITE_LOCK

	return true;
}
