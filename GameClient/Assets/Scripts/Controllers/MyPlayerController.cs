using Google.Protobuf.Protocol;
using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UIElements;
using static Define;

public class MyPlayerController : PlayerController
{
    bool        _moveKeyPressed = false;
    float       _movePacketSendTick = 0.2f;
    Vector3     _moveDirection;
    float       _moveYaw = 0.0f;

    Vector3     _nextInputDir = Vector3.zero;   
    Vector3     _prevInputDir = Vector3.zero;

    protected override void Init()
    {
        base.Init();

        StartCoroutine(CoStartMovePacketSend());
    }

    protected override void UpdateController()
    {
        UpdateInput();

        base.UpdateController();

        {
            bool forceMovePacketSend = false;

            if (_prevInputDir != _nextInputDir)
            {
                forceMovePacketSend = true;
                _prevInputDir = _nextInputDir;
            }

            //if (_nextInputDir == Vector3.zero)
            //    MoveState = MoveState.Idle;
            //else
            //    MoveState = MoveState.Run;

            if (forceMovePacketSend)
            {
                C_MOVE forceMovePacket = new C_MOVE();
                forceMovePacket.PlayerInfo = DestInfo;
                forceMovePacket.PlayerInfo.State = MoveState;
                Managers.Network.Send(forceMovePacket);
            }
        }
    }

    //float _idle_run_ratio = 0.0f;

    IEnumerator CoStartMovePacketSend()
    {
        // 대기 시간
        while(true)
        {
            yield return new WaitForSeconds(_movePacketSendTick);
            C_MOVE movePacket = new C_MOVE();
            movePacket.PlayerInfo = Info;
            Managers.Network.Send(movePacket);
        }
    }

    protected override void MoveToNextPos()
    {
        if (_moveKeyPressed == false)
        {
            MoveState = MoveState.Idle;
            return;
        }
    }


    void UpdateInput()
    {
        //  local -> world
        //transform.TransformDirection
        //  world -> local
        //transform.InverseTransformDirection

        _moveKeyPressed = true;
        Vector3 destPos = Vector3.zero;
        Vector3 moveDir = Vector3.zero;

        if (Input.GetKey(KeyCode.W))
        {
            moveDir = Camera.main.transform.forward;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            moveDir = Camera.main.transform.forward;
            moveDir.x = -moveDir.x;
            moveDir.z = -moveDir.z;
        }
        else if (Input.GetKey(KeyCode.A))
        {
            moveDir = Camera.main.transform.right;
            moveDir.x = -moveDir.x;
            moveDir.z = -moveDir.z;
        }
        else if (Input.GetKey(KeyCode.D))
        {
            moveDir = Camera.main.transform.right;
        }
        else
            _moveKeyPressed = false;

        if (_moveKeyPressed)
        {
            _moveDirection = Vector3.zero;
            moveDir.y = 0;

            _nextInputDir = moveDir;

            _moveDirection = moveDir;

            Quaternion q = Quaternion.LookRotation(moveDir);    
            _moveYaw = q.eulerAngles.y;

            MoveState = MoveState.Run;
            destPos = transform.position + moveDir;

            _destInfo.X = destPos.x;
            _destInfo.Y = destPos.y;
            _destInfo.Z = destPos.z;
            _destInfo.Yaw = _moveYaw;
        }
    }
}
