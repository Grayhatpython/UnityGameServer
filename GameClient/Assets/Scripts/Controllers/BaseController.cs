using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using static Define;

public abstract class BaseController : MonoBehaviour
{
    protected float                 _speed = 5f;

    protected Define.CreatureState  _state = Define.CreatureState.Idle;
    protected Vector3               _destPos;
    protected Animator              _animator;

    public virtual Define.CreatureState State
    {
        get { return _state; }
        set
        {
            _state = value;
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
        switch (_state)
        {
            case Define.CreatureState.Idle:
                UpdateIdle();
                break;
            case Define.CreatureState.Moving:
                UpdateMoving();
                break;
            case Define.CreatureState.Skill:
                UpdateSkill();
                break;
            case Define.CreatureState.Die:
                UpdateDie();
                break;
        }
    }

    protected virtual void Init()
    {
        _animator = GetComponent<Animator>();

        //UpdateAnimation();
    }

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

    protected virtual void UpdateIdle() { }
    protected virtual void UpdateMoving()
    {
        Vector3 moveDir = _destPos - transform.position;

        float distance = moveDir.magnitude;
        if(distance < 0.1f)
        {
            transform.position = _destPos;
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
