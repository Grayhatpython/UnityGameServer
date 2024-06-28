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

    protected override void Init()
    {
        base.Init();

        StartCoroutine(CoStartMovePacketSend());
    }

    protected override void UpdateController()
    {
        UpdateInput();

        base.UpdateController();
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
            _moveDirection = moveDir;
            Quaternion q = Quaternion.FromToRotation(transform.position, transform.position + _moveDirection);
            _moveYaw = q.eulerAngles.y;

            MoveState = MoveState.Run;
            moveDir.y = 0;
            destPos = transform.position + moveDir;

            _destInfo.X = destPos.x;
            _destInfo.Y = destPos.y;
            _destInfo.Z = destPos.z;
        }
    }
}
