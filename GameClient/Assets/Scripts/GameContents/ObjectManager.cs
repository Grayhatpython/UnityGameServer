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

    public void Spawn(Protocol.ObjectInfo objectInfo, bool isMyPlayer)
    {
        //if (MyPlayer != null)
        //    return;

        //  이미 있는 경우 스킵
        if (_objects.ContainsKey(objectInfo.ObjectId))
            return;

        if (isMyPlayer)
        {
            GameObject go = Managers.Resource.Instantiate("MyPlayer");
            go.transform.position = new Vector3(objectInfo.PositionInfo.X, objectInfo.PositionInfo.Y, objectInfo.PositionInfo.Z);
            _objects.Add(objectInfo.ObjectId, go);

            MyPlayer = go.GetComponent<MyPlayerController>();

            MyPlayer.Info = objectInfo.PositionInfo;

            CameraController cc = Camera.main.gameObject.GetComponent<CameraController>();
            cc.MyPlayer = go;
        }
        else
        {
            GameObject go = Managers.Resource.Instantiate("Player");
            go.transform.position = new Vector3(objectInfo.PositionInfo.X, objectInfo.PositionInfo.Y, objectInfo.PositionInfo.Z);
            _objects.Add(objectInfo.ObjectId, go);

            PlayerController pc = go.GetComponent<PlayerController>();

            pc.Info = objectInfo.PositionInfo;
        }

    }   

    public void Spawn(S_ENTER_GAME enterGamePacket)
    {
        Spawn(enterGamePacket.Player, true);
    }

    public void Spawn(S_SPAWN spawnPacket)
    {
        foreach (Protocol.ObjectInfo objectInfo in spawnPacket.Objects)
            Spawn(objectInfo, false);
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

    //  TEMP
    public void Move(S_MOVE movePacket)
    {
        GameObject go;

        if (_objects.TryGetValue(movePacket.PositionInfo.ObjectId, out go) == false)
            return;

        if (MyPlayer.Info.ObjectId == movePacket.PositionInfo.ObjectId)
            return;

        //  TEST
        PlayerController pc = go.GetComponent<PlayerController>();
        if (pc == null)
            return;

        pc.DestInfo = movePacket.PositionInfo;    
        pc.MoveState = movePacket.PositionInfo.State;
    }

    public void Clear()
    {
        foreach(GameObject go in _objects.Values)
            Managers.Resource.Destroy(go); 

        _objects.Clear();
    }
}
