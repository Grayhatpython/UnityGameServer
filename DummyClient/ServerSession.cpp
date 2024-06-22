#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

ServerSession::~ServerSession()
{
	cout << "~ServerSession" << endl;
}

void  ServerSession::OnConnected()
{
	//	클라이언트 접속 후 서버로 로그인 패킷 전송
	/*Protocol::C_LOGIN loginPacket;
	
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPacket);
	Send(sendBuffer);*/

	Protocol::C_CONNECTED connectedPacket;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(connectedPacket);
	Send(sendBuffer);
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSessionRef();
	ServerPacketHandler::PacketProcessing(session, buffer, len);
}

void  ServerSession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}

void  ServerSession::OnDisconnected()
{

}