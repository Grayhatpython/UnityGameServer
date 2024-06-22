using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace PacketGenerator
{
    internal class GameClient_Unity_PacketGenerate
    {
        string clientRegister = string.Empty;
        string serverRegister = string.Empty;
        string packetEnums = string.Empty;

        List<string> clientToServerPacketIds = new List<string>();
        List<string> serverToClientPacketIds = new List<string>();
        List<string> packets = new List<string>();

        string serverPacketRegister = "";
        string serverPacketHandler = "";
        string serverPacketIdTableRegister = "";

        public void Generate()
        {
#if DEBUG
            string protoPath = "../../../../Common/Protoc/Protocol.proto";
#else
            string protoPath = "./Protocol.proto";
#endif
            string[] text = File.ReadAllLines(protoPath);

            PacketExtraction(text);
            ParsePacketEnums();
            ParseServerPacketHandler();

            serverPacketHandler = string.Format(GameClient_Unity_PacketFormat.packetHandlerFormat, packetEnums, serverPacketRegister, serverPacketIdTableRegister);
            File.WriteAllText("ServerPacketHandler.cs", serverPacketHandler);
        }

        public static string FirstCharToUpper(string input)
        {
            if (string.IsNullOrEmpty(input))
                return "";
            return input[0].ToString().ToUpper() + input.Substring(1).ToLower();
        }


        void PacketExtraction(string[] text)
        {
            if (text.Length > 0)
            {
                for (int i = 0; i < text.Length; i++)
                {
                    if (text[i].Contains("message") == false)
                        continue;
                    else
                    {
                        //  한라인을 공백 기준으로 나눔
                        string[] temp = text[i].Split('\x020');
                        string packetId = temp[1].ToUpper();

                        if (packetId.FirstOrDefault() == 'C')
                            clientToServerPacketIds.Add(packetId);
                        else
                            serverToClientPacketIds.Add(packetId);

                        packets.Add(packetId);
                    }
                }
            }
        }

        void ParsePacketEnums()
        {
            int packetStartNumber = 1;

            for (int i = 0; i < packets.Count; i++)
            {
                packetEnums += '\t';
                packetEnums += string.Format(GameClient_Unity_PacketFormat.packetEnumFormat, packets[i], packetStartNumber++);
                packetEnums += Environment.NewLine;
            }
        }

        //  Clinet Generate
        void ParseServerPacketHandler()
        {
            //  S_PACKET
            foreach (string packet in serverToClientPacketIds)
            {
                serverPacketRegister += string.Format(GameClient_Unity_PacketFormat.packetHandlerRegisterFormat, packet);
                serverPacketRegister += Environment.NewLine;
            }

            foreach (string packet in clientToServerPacketIds)
            {
                serverPacketIdTableRegister += string.Format(GameClient_Unity_PacketFormat.packetNameToPacketIdTableFormat, packet);
                serverPacketIdTableRegister += Environment.NewLine;
            }
        }
    }
}
