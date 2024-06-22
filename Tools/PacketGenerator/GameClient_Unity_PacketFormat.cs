using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
	internal class GameClient_Unity_PacketFormat
	{
		// {0} 패킷 등록
		public static string packetHandlerFormat =
@"using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;

public enum PacketID
{{
	{0}
}}

class ServerPacketHandler
{{
	#region Singleton
	static ServerPacketHandler _instance = new ServerPacketHandler();
	public static ServerPacketHandler Instance {{ get {{ return _instance; }} }}
	#endregion

	ServerPacketHandler()
	{{
		Register();
	}}

	Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IMessage>> _makePacket = new Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IMessage>>();
	Dictionary<ushort, Action<PacketSession, IMessage>> _packetHandler = new Dictionary<ushort, Action<PacketSession, IMessage>>();
		
	public Dictionary<string, ushort> PacketNameToPacketIdTable  {{ get; private set; }} = new Dictionary<string, ushort>();

	public void Register()
	{{
		{1}

		{2}
	}}

	public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer, Action<PacketSession, IMessage, ushort> onRecvFunc = null)
	{{
		ushort count = 0;

		ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
		count += 2;
		ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
		count += 2;

		Func<PacketSession, ArraySegment<byte>, IMessage> func = null;
		if (_makePacket.TryGetValue(id, out func))
		{{
			IMessage packet = func.Invoke(session, buffer);
			//	PacketQueue Push
			if (onRecvFunc != null)
				onRecvFunc.Invoke(session, packet, id);
			else
				HandlePacket(session, packet, id);
		}}
	}}

	T MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T : IMessage, new()
	{{
		T packet = new T();
		packet.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);
		return packet;
	}}

	public void HandlePacket(PacketSession session, IMessage packet, ushort id)
	{{
        Action<PacketSession, IMessage> action = null;
        if (_packetHandler.TryGetValue(id, out action))
            action.Invoke(session, packet);
    }}
}}";

        // {0} 패킷 이름
        public static string quotes = "\"";
		public static string packetHandlerRegisterFormat =
@"		
		_makePacket.Add((ushort)PacketID.PACKET_{0}, MakePacket<{0}>);
		_packetHandler.Add((ushort)PacketID.PACKET_{0}, PacketHandler.{0}_PacketHandler);";

		public static string packetNameToPacketIdTableFormat = 
@"
        PacketNameToPacketIdTable.Add(" + quotes + "{0}" + quotes + ", (ushort)PacketID.PACKET_{0});";

        public static string packetEnumFormat =
@"PACKET_{0} = {1},";
	}
}
