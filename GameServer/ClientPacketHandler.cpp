#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "Room.h"
#include "Player.h"
#include "GameObjectManager.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	return true;
}

bool C_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet)
{
	//	Client -> Login Server -> Game Server
	//	������ Ȯ���ϱ� ���� Redis? 
	//	DB���� Account ����

	Protocol::S_LOGIN loginPacket;

	for (int32 i = 0; i < 2; i++)
	{
		auto player = loginPacket.add_players();
		player->set_x(Utils::GetRandom(0.f, 100.f));
		player->set_y(0);
		player->set_z(Utils::GetRandom(0.f, 100.f));
		player->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	loginPacket.set_successed(true);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPacket);
	session->Send(sendBuffer);

	return true;
}

bool C_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& packet)
{
	//	�÷��̾� ����
	auto clientSession = static_pointer_cast<ClientSession>(session);
	auto player = GameObjectManager::CreatePlayer(clientSession);

	if (player == nullptr)
		return false;

	//	Room Enter
	GRoom->PushJob(&Room::Enter, true, player);

	return true;
}

bool C_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LEAVE_GAME& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);
	auto player = clientSession->_player;

	if (player == nullptr)
		return false;

	auto room = player->_room.lock();
	if (room == nullptr)
		return false;

	room->PushJob(&Room::Leave, true, player);

	return true;
}

bool C_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_MOVE& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);
	auto player = clientSession->_player;

	if (player == nullptr)
		return false;

	auto room = player->_room.lock();
	if (room == nullptr)
		return false;

	room->PushJob(&Room::Move, true, packet);

	return true;
}

bool C_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHAT& packet)
{
	return true;
}




/*

message C_LOGIN
{

}

message S_LOGIN
{
	bool success = 1;
	repeated Player players = 2;	//	���̵� �߱� ��
}

message C_ENTER_GAME
{
	uint64 playerIndex = 1;	//	���° �÷��̾�� ���� �ҷ�?
}

message S_ENTER_GAME
{
	bool success = 1;
}

message C_CHAT
{
	string msg = 1;
}

message S_CHAT
{
	uint64 playerId = 1;
	string msg = 2;
}

bool C_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet)
{
	//	Ŭ���̾�Ʈ���� �α��� ��Ŷ�� ���� �� ó��
	auto clientSession = static_pointer_cast<ClientSession>(session);

	//	Vaildation Check

	Protocol::S_LOGIN loginPacket;
	loginPacket.set_success(true);

	//	DB���� �÷��̾� ������ ���´�.
	//	ClientSession �÷��̾� ������ ���� ( in Memory )

	//	ID �߱� ( in Game Id )
	static Atomic<uint64> idGenerator = 1;

	{
		auto player = loginPacket.add_players();
		player->set_name(u8"db�����ܾ���̸�1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);
		player->set_id(idGenerator);

		auto playerRef = MakeShared<Player>();
		playerRef->_playerId = idGenerator++;
		playerRef->_name = player->name();
		playerRef->_type = player->playertype();
		playerRef->_ownerSession = clientSession;	//	Cycle;

		clientSession->_players.push_back(playerRef);
	}

	{
		auto player = loginPacket.add_players();
		player->set_name(u8"db�����ܾ���̸�2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);
		player->set_id(idGenerator);

		auto playerRef = MakeShared<Player>();
		playerRef->_playerId = idGenerator++;
		playerRef->_name = player->name();
		playerRef->_type = player->playertype();
		playerRef->_ownerSession = clientSession;	//	Cycle;

		clientSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPacket);
	session->Send(sendBuffer);

	return true;
}

bool C_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& packet)
{
	//	Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� ���� �� ó��
	auto clientSession = static_pointer_cast<ClientSession>(session);

	auto playerIndex = packet.playerindex();
	//	Vaildation Check

	clientSession->_currentPlayer = clientSession->_players[playerIndex];	//	Read only;
	clientSession->_room = GRoom;

	GRoom->PushJob(&Room::Enter, clientSession->_currentPlayer);

	Protocol::S_ENTER_GAME enterGamePacket;
	enterGamePacket.set_success(true);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
	clientSession->_currentPlayer->_ownerSession->Send(sendBuffer);

	return true;
}

bool C_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHAT& packet)
{
	cout << packet.msg() << endl;

	Protocol::S_CHAT charPacket;
	charPacket.set_msg(packet.msg());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(charPacket);

	//	�ϰ��� �о� ������ Ÿ�̹��� ������ �� ����.
	GRoom->PushJob(&Room::Broadcast, sendBuffer);

	//	�������� �κ��� Lock
	//GRoom.Broadcast(sendBuffer);	//	WRITE_LOCK

	return true;
}


bool C_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CONNECTED& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);

	//	ID �߱� ( in Game Id )
	static Atomic<uint64> idGenerator = 1;

	auto player = MakeShared<Player>();
	player->_info.set_id(idGenerator++);
	//player->_playerId = idGenerator;
	player->_info.set_posx(0);
	player->_info.set_posy(0);
	player->_info.set_posz(0);
	player->_ownerSession = clientSession;

	//clientSession->_players.push_back(player);
	clientSession->_currentPlayer = player;
	clientSession->_room = GRoom;

	GRoom->PushJob(&Room::Enter, true, clientSession->_currentPlayer);

	return true;
}

//	���� �ʿ�.. �ӽ� ����������
bool C_LEAVEGAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LEAVEGAME& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);

	GRoom->PushJob(&Room::Leave, true, clientSession->_currentPlayer);

	return true;
}

bool C_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_MOVE& packet)
{
	auto clientSession = static_pointer_cast<ClientSession>(session);

	//cout << packet.posx() << " , " << packet.posy() << " , " << packet.posz() << endl;

	GRoom->PushJob(&Room::Move, true, clientSession, &packet);
	return true;
}

*/