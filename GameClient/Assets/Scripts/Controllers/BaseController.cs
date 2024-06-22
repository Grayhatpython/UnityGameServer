using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using static Define;

public abstract class BaseController : MonoBehaviour
{
    float _speed = 5f;

    protected Define.CreatureState _state = Define.CreatureState.Idle;
    protected Vector3 _destPos;
    protected Define.MoveDir _dir = Define.MoveDir.Down;
    protected Animator _animator;

    public virtual Define.CreatureState State
    {
        get { return _state; }
        set
        {
            _state = value;

            switch(_state)
            {
                case Define.CreatureState.Idle:
                    _animator.CrossFade("IDLE", 0.1f);
                    break;
                case Define.CreatureState.Moving:
                    _animator.Play("RUN");
                    //_animator.CrossFade("RUN", 1.0f);
                    break;
                case Define.CreatureState.Skill:
                    break;
                case Define.CreatureState.Die:
                    break;
            }
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
        transform.rotation = Quaternion.Euler(0, 180, 0);

        UpdateAnimation();
    }

    protected virtual void UpdateAnimation()
    {
        if (_animator == null)
            return;

        if(State == Define.CreatureState.Idle)
        {
            _animator.CrossFade("IDLE", 0.1f);
        }
        else if (State == Define.CreatureState.Moving)
        {
            _animator.Play("RUN");
            //_animator.CrossFade("RUN", 1.0f);
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
            switch (_dir)
            {
                case Define.MoveDir.Up:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.forward), 0.2f);
                    break;
                case Define.MoveDir.UpLeft:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.forward + Vector3.left), 0.2f);
                    break;
                case Define.MoveDir.Left:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.left), 0.2f);
                    break;
                case Define.MoveDir.DownLeft:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.back + Vector3.left), 0.2f);
                    break;
                case Define.MoveDir.Down:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.back), 0.2f);
                    break;
                case Define.MoveDir.DownRight:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.back + Vector3.right), 0.2f);
                    break;
                case Define.MoveDir.Right:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.right), 0.2f);
                    break;
                case Define.MoveDir.UpRight:
                    transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(Vector3.forward + Vector3.right), 0.2f);
                    break;
            }

            transform.position += moveDir.normalized * _speed * Time.deltaTime;
            State = CreatureState.Moving;
        }
    }

    protected virtual void MoveToNextPos()
    {

    }

    protected virtual void UpdateSkill() { }
    protected virtual void UpdateDie() { }
}
