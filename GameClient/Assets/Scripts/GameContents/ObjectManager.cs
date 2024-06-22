using Google.Protobuf.Protocol;
using Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class ObjectManager
{
    public MyPlayerController MyPlayer { get; set; }
    Dictionary<UInt64, GameObject> _objects= new Dictionary<UInt64, GameObject>();

    public void Spawn(Protocol.PlayerInfo playerInfo, bool isMyPlayer)
    {
        //if (MyPlayer != null)
        //    return;

        //  이미 있는 경우 스킵
        if (_objects.ContainsKey(playerInfo.ObjectId))
            return;


        if (isMyPlayer)
        {
            GameObject go = Managers.Resource.Instantiate("MyPlayer");
            go.transform.position = new Vector3(playerInfo.X, playerInfo.Y, playerInfo.Z);
            _objects.Add(playerInfo.ObjectId, go);

            MyPlayer = go.GetComponent<MyPlayerController>();
        }
        else
        {
            GameObject go = Managers.Resource.Instantiate("Player");
            go.transform.position = new Vector3(playerInfo.X, playerInfo.Y, playerInfo.Z);
            _objects.Add(playerInfo.ObjectId, go);

            PlayerController pc = go.GetComponent<PlayerController>();
        }

    }   

    public void Spawn(S_ENTER_GAME enterGamePacket)
    {
        Spawn(enterGamePacket.Player, true);
    }

    public void Spawn(S_SPAWN spawnPacket)
    {
        foreach (Protocol.PlayerInfo playerInfo in spawnPacket.Players)
            Spawn(playerInfo, false);
    }

    public void Despawn(UInt64 objectId)
    {
        if (_objects.ContainsKey(objectId) == false)
            return;

        GameObject go = _objects[objectId];
        if (go == null)
            return;

        _objects.Remove(objectId);  
        Managers.Resource.Destroy(go);
    }

    public void Despawn(S_DESPAWN deSpawnPacket)
    {
        foreach (UInt64 objectId in deSpawnPacket.ObjectIds)
            Despawn(objectId);
    }

    public void Clear()
    {
        foreach(GameObject go in _objects.Values)
            Managers.Resource.Destroy(go); 

        _objects.Clear();
    }
}
