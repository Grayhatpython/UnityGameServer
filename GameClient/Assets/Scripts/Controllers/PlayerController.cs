using Google.Protobuf.Protocol;
using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class PlayerController : BaseController
{
    protected PlayerInfo _info = new PlayerInfo();

    public PlayerInfo Info
    {
        get { return _info; }
        set
        {
            if (_info.Equals(value))
                return;

            _info = value;
            transform.position = new Vector3(_info.X, _info.Y, _info.Z);
            transform.rotation = Quaternion.Euler(new Vector3(0.0f, _info.Yaw, 0.0f));
        }
    }

    public override Define.CreatureState State
    {
        get { return _state; }
        set
        {
            _state = value;

            switch (_state)
            {
                case Define.CreatureState.Idle:
                    _animator.CrossFade("IDLE", 0.2f);
                    break;
                case Define.CreatureState.Moving:
                    _animator.CrossFade("RUN", 0.001f);
                    break;
            }
        }
    }

    protected override void UpdateController()
    {
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
