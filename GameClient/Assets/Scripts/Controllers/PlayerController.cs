using Google.Protobuf.Protocol;
using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class PlayerController : BaseController
{
    public override MoveState MoveState
    {
        get { return _positionInfo.State; }
        set
        {
            if (_positionInfo.State.Equals(value))
                return;

            _positionInfo.State = value;

            switch (_positionInfo.State)
            {
                case MoveState.Idle:
                    _animator.CrossFade("IDLE", 0.2f);
                    break;
                case MoveState.Run:
                    _animator.CrossFade("RUN", 0.001f);
                    break;
            }
        }
    }

    protected override void UpdateController()
    {
        //  TEMP
        {
            //  매 프레임 좌표 회전 갱신
            _positionInfo.X = transform.position.x;
            _positionInfo.Y = transform.position.y;
            _positionInfo.Z = transform.position.z;
            _positionInfo.Yaw = transform.rotation.eulerAngles.y;
        }

        base.UpdateController();
    }

    protected override void UpdateDie()
    {

    }

    protected override void UpdateIdle()
    {

    }

    protected override void Init()
    {
        base.Init();
    }
}
