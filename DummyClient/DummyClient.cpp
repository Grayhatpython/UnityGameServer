#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

int main()
{
	ServerPacketHandler::Initialize();

	this_thread::sleep_for(500ms);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession >, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	//chatPacket.set_msg(u8"Hello World!");
	//auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPacket);

	Protocol::C_MOVE movePacket;

	while (true)
	{
		auto randomX = Utils::GetRandom(-50, 50);
		auto randomZ = Utils::GetRandom(-50, 50);
		movePacket.set_posx(static_cast<float>(randomX));
		movePacket.set_posy(0);
		movePacket.set_posz(static_cast<float>(randomZ));
		
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(movePacket);
		service->Broadcast(sendBuffer);
		std::this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}