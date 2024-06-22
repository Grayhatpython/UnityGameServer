using Google.Protobuf.Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class PlayerController : BaseController
{
    bool _isMovekeyPressed = false;

    protected override void UpdateController()
    {
        switch(State)
        {
            case Define.CreatureState.Idle:
            case Define.CreatureState.Moving:
                OnKeyBoardEvent();
                break;
        }

        base.UpdateController();
    }

    protected override void UpdateDie()
    {

    }

    protected override void UpdateIdle()
    {
        if (_isMovekeyPressed)
        {
            State = Define.CreatureState.Moving;
        }
    }

    protected override void MoveToNextPos()
    {
        if(_isMovekeyPressed == false)
        {
            State = Define.CreatureState.Idle;
            return;
        }

        Vector3 destPos = transform.position;

        switch (_dir)
        {
            case Define.MoveDir.Up:
                destPos += Vector3.forward;
                break;
            case Define.MoveDir.UpLeft:
                destPos += (Vector3.forward + Vector3.left);
                break;
            case Define.MoveDir.Left:
                destPos += Vector3.left;
                break;
            case Define.MoveDir.DownLeft:
                destPos += (Vector3.back + Vector3.left);
                break;
            case Define.MoveDir.Down:
                destPos += Vector3.back;
                break;
            case Define.MoveDir.DownRight:
                destPos += (Vector3.back + Vector3.right);
                break;
            case Define.MoveDir.Right:
                destPos += Vector3.right;
                break;
            case Define.MoveDir.UpRight:
                destPos += (Vector3.forward + Vector3.right);
                break;
        }

        _destPos = destPos;
    }

    void OnKeyBoardEvent()
    {

        //_isMovekeyPressed = true;

        //Input.GetAxis("Horizontal");
        //if (Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.D))
        //{
        //    _dir = Define.MoveDir.UpRight;
        //}
        //else if (Input.GetKey(KeyCode.S))
        //{
        //    _dir = Define.MoveDir.Down;
        //    if (Input.GetKey(KeyCode.D))
        //        _dir = Define.MoveDir.DownRight;
        //    else if (Input.GetKey(KeyCode.A))
        //        _dir = Define.MoveDir.DownLeft;
        //}
        //else if (Input.GetKey(KeyCode.A))
        //{
        //    _dir = Define.MoveDir.Left;
        //    if (Input.GetKey(KeyCode.W))
        //        _dir = Define.MoveDir.UpLeft;
        //    else if (Input.GetKey(KeyCode.S))
        //        _dir = Define.MoveDir.DownLeft;
        //}
        //else if (Input.GetKey(KeyCode.D))
        //{
        //    _dir = Define.MoveDir.Right;
        //    if (Input.GetKey(KeyCode.W))
        //        _dir = Define.MoveDir.UpRight;
        //    else if (Input.GetKey(KeyCode.S))
        //        _dir = Define.MoveDir.DownRight;
        //}
        //else
        //    _isMovekeyPressed = false;
    }

    protected override void Init()
    {
        base.Init();

        //Managers.Input.KeyAction -= OnKeyBoardEvent;
        //Managers.Input.KeyAction += OnKeyBoardEvent;
    }
}
