using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using static Define;

public abstract class BaseController : MonoBehaviour
{
    protected float                 _speed = 5f;
    protected PlayerInfo            _info = new PlayerInfo();

    //  TEMP : ¸ñÀûÁö
    protected PlayerInfo _destInfo = new PlayerInfo();

    protected Animator              _animator;

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

    public PlayerInfo DestInfo
    {
        get { return _destInfo; }
        set
        {
            if (_destInfo.Equals(value))
                return;

            _destInfo = value;
            transform.position = new Vector3(_destInfo.X, _destInfo.Y, _destInfo.Z);
            transform.rotation = Quaternion.Euler(new Vector3(0.0f, _destInfo.Yaw, 0.0f));
        }
    }

    public virtual MoveState MoveState
    {
        get { return _info.State; }
        set 
        {
            if (_info.State.Equals(value))
                return;

            _info.State = value;    
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        Init();
    }

    // Update is called once per frame
    void Update()
    {
        UpdateController();
    }

    protected virtual void UpdateController()
    {
        switch (MoveState)
        {
            case MoveState.Idle:
                UpdateIdle();
                break;
            case MoveState.Run:
                UpdateMoving();
                break;
        }
    }

    protected virtual void Init()
    {
        _animator = GetComponent<Animator>();

        //  TEMP
        {
            _destInfo.X = transform.position.x;
            _destInfo.Y = transform.position.y;
            _destInfo.Z = transform.position.z;
            _destInfo.Yaw = transform.rotation.eulerAngles.y;
        }

        //UpdateAnimation();
    }

    /*
    protected virtual void UpdateAnimation()
    {
        if (_animator == null)
            return;

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
    */

    protected virtual void UpdateIdle() { }
    protected virtual void UpdateMoving()
    {
        Vector3 destPos = new Vector3(_destInfo.X, _destInfo.Y, _destInfo.Z);
        Vector3 moveDir = destPos - transform.position;

        float distance = moveDir.magnitude;
        if(distance < 0.1f)
        {
            transform.position = destPos;
            MoveToNextPos();
        }
        else
        {
            float moveDist = Mathf.Clamp(_speed * Time.deltaTime, 0, moveDir.magnitude);
            transform.position += moveDir.normalized * moveDist;
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(moveDir), 20 * Time.deltaTime);
        }
    }

    protected virtual void UpdateSkill() { }
    protected virtual void UpdateDie() { }
    protected virtual void MoveToNextPos() { }
}
