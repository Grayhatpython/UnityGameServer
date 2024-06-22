﻿using DummyClient;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

class PacketHandler
{
    public static void S_LOGIN_PacketHandler(PacketSession session, IMessage packet)
    {
        S_LOGIN loginPacket = packet as S_LOGIN;
        ServerSession serverSession = session as ServerSession;

        //  로비에서 캐릭터 선택해서 캐릭터 넘버(인덱스) 전송
        foreach(Protocol.PlayerInfo player in loginPacket.Players)
        {
            ;
        }

        C_ENTER_GAME enterGamePacket = new C_ENTER_GAME();
        enterGamePacket.PlayerIndex = 0;
        Managers.Network.Send(enterGamePacket);
    }

    public static void S_ENTER_GAME_PacketHandler(PacketSession session, IMessage packet)
    {
        S_ENTER_GAME enterGamePacket = packet as S_ENTER_GAME;
        ServerSession serverSession = session as ServerSession;

        Managers.Object.Spawn(enterGamePacket);
    }

    public static void S_LEAVE_GAME_PacketHandler(PacketSession session, IMessage packet)
    {
        S_LEAVE_GAME leaveGamePacket = packet as S_LEAVE_GAME;
        ServerSession serverSession = session as ServerSession;

        //  게임이 종료 ? 로비로 이동 ?
        Managers.Object.Clear();
    }

    public static void S_SPAWN_PacketHandler(PacketSession session, IMessage packet)
    {
        S_SPAWN spawnPacket = packet as S_SPAWN;
        ServerSession serverSession = session as ServerSession;

        Managers.Object.Spawn(spawnPacket);
    }

    public static void S_CHAT_PacketHandler(PacketSession session, IMessage packet)
    {
        S_CHAT chatPacket = packet as S_CHAT;
        ServerSession serverSession = session as ServerSession;

    }

    public static void S_DESPAWN_PacketHandler(PacketSession session, IMessage packet)
    {
        S_DESPAWN despawnPacket = packet as S_DESPAWN;
        ServerSession serverSession = session as ServerSession;

        Managers.Object.Despawn(despawnPacket);
    }
}
