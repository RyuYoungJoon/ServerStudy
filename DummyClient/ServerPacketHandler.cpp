#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);


	return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return true;

	if (pkt.players().size() == 0)
	{
		// ĳ���� ����â
	}

	// ���� UI ��ư ������ ���� ����
	Protocol::C_ENTER_GAME enterGamePacket;
	enterGamePacket.set_playerindex(0);		// ù��° ĳ���ͷ� ����.
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePacket);
	session->Send(sendBuffer);


	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	// TODO : 
	return false;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;
	return false;
}