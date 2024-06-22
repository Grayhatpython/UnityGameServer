using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    internal class ServerAndDummyPacketGenerate
    {
        string packetEnums = string.Empty;

        string clientPacketFunction = "";
        string clientPacketRegister = "";
        string clientMakeSendBuffer = "";

        string serverPacketFunction = "";
        string serverPacketRegister = "";
        string serverMakeSendBuffer = "";

        string serverPacketHandler = "";
        string clientPacketHandler = "";

        List<string> packets = new List<string>();
        List<string> clientToServerPacketIds = new List<string>();
        List<string> serverToClientPacketIds = new List<string>();

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
            ParseClientPacketHandler();
            ParseServerPacketHandler();

            serverPacketHandler = string.Format(ServerAndDummyPacketFormat.fileFormat, packetEnums, serverPacketFunction, "ServerPacketHandler", serverPacketRegister, serverMakeSendBuffer);
            clientPacketHandler = string.Format(ServerAndDummyPacketFormat.fileFormat, packetEnums, clientPacketFunction, "ClientPacketHandler", clientPacketRegister, clientMakeSendBuffer);

            File.WriteAllText("ServerPacketHandler.h", serverPacketHandler);
            File.WriteAllText("ClientPacketHandler.h", clientPacketHandler);
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
                packetEnums += string.Format(ServerAndDummyPacketFormat.packetEnumFormat, packets[i], packetStartNumber++);
                packetEnums += Environment.NewLine;
            }
        }

        //  Game Server Generate
        void ParseClientPacketHandler()
        {
            //  C_PACKET
            foreach (string packet in clientToServerPacketIds)
            {
                clientPacketFunction += string.Format(ServerAndDummyPacketFormat.packetProcessingFunctionFormat, packet, packet);
                clientPacketFunction += Environment.NewLine;

                clientPacketRegister += "\t\t";
                clientPacketRegister += string.Format(ServerAndDummyPacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                clientPacketRegister += Environment.NewLine;
            }

            //  S_PACKET
            foreach (string packet in serverToClientPacketIds)
            {
                clientMakeSendBuffer += '\t';
                clientMakeSendBuffer += string.Format(ServerAndDummyPacketFormat.makePacketToSendBufferFormat, packet, packet);
                clientMakeSendBuffer += Environment.NewLine;
            }
        }

        //  Clinet Generate
        void ParseServerPacketHandler()
        {
            //  S_PACKET
            foreach (string packet in serverToClientPacketIds)
            {
                serverPacketFunction += string.Format(ServerAndDummyPacketFormat.packetProcessingFunctionFormat, packet, packet);
                serverPacketFunction += Environment.NewLine;

                serverPacketRegister += "\t\t";
                serverPacketRegister += string.Format(ServerAndDummyPacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                serverPacketRegister += Environment.NewLine;
            }

            //  C_PACKET
            foreach (string packet in clientToServerPacketIds)
            {
                serverMakeSendBuffer += '\t';
                serverMakeSendBuffer += string.Format(ServerAndDummyPacketFormat.makePacketToSendBufferFormat, packet, packet);
                serverMakeSendBuffer += Environment.NewLine;
            }
        }
    }
}
