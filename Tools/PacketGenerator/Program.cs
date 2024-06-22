
namespace PacketGenerator
{
    internal class Program
    {

        static void Main(string[] args)
        {

            ServerAndDummyPacketGenerate serverAndDummyPacketGenerate = new ServerAndDummyPacketGenerate();
            serverAndDummyPacketGenerate.Generate();

            GameClient_Unity_PacketGenerate gameClient_Unity_PacketGenerate = new GameClient_Unity_PacketGenerate();
            gameClient_Unity_PacketGenerate.Generate();

        }
    }
}