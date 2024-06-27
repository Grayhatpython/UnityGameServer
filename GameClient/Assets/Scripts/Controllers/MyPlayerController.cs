using Google.Protobuf.Protocol;
using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;
using static Define;

public class MyPlayerController : PlayerController
{
    bool        _moveKeyPressed = false;
    float       _movePacketSendTick = 0.2f;
    bool        _isMoving = false;

    protected override void Init()
    {
        base.Init();

        StartCoroutine(CoStartMovePacketSend());
    }

    protected override void UpdateController()
    {
        //  TEMP
        {
            //  매 프레임 좌표 회전 갱신
            _info.X = transform.position.x;
            _info.Y = transform.position.y;
            _info.Z = transform.position.z;
            _info.Yaw = transform.rotation.eulerAngles.y;
            //Debug.Log(transform.rotation.y);
            //Debug.Log(transform.rotation.eulerAngles.y);
        }

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
            State = Define.CreatureState.Idle;
            return;
        }

        _isMoving = false;
        transform.position = _destPos;
    }


    void UpdateInput()
    {
        //  local -> world
        //transform.TransformDirection
        //  world -> local
        //transform.InverseTransformDirection
        if (_isMoving == true)
            return;

        _moveKeyPressed = true;

        Vector3 destPos = Vector3.zero;

        if (Input.GetKey(KeyCode.W))
        {
            //transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.forward), 0.2f);
            //transform.Translate(Vector3.forward * Time.deltaTime * _speed);
            //transform.position += Vector3.forward * Time.deltaTime * _speed;

            destPos = Vector3.forward  + transform.position;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            //transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.back), 0.2f);
            //transform.Translate(Vector3.forward * Time.deltaTime * _speed);
            //transform.position += Vector3.back / 2 * Time.deltaTime * _speed;
            destPos = Vector3.back  + transform.position;

        }
        else if (Input.GetKey(KeyCode.A))
        {
            //transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.left), 0.2f);
            //transform.Translate(Vector3.forward * Time.deltaTime * _speed);
            //transform.position += Vector3.left / 2 * Time.deltaTime * _speed;
            destPos = Vector3.left + transform.position;

        }
        else if (Input.GetKey(KeyCode.D))
        {
            //transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.right), 0.2f);
            //transform.Translate(Vector3.forward * Time.deltaTime * _speed);
            //transform.position += Vector3.right / 2 * Time.deltaTime * _speed;
            destPos = Vector3.right + transform.position;

        }
        else
            _moveKeyPressed = false;

        if (_moveKeyPressed)
        {
            State = Define.CreatureState.Moving;
            _destPos = destPos;
            _isMoving = true;
        }
    }
}
