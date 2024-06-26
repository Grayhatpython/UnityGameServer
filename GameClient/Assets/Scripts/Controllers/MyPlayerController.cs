using Google.Protobuf.Protocol;
using Protocol;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;
using static Define;

public class MyPlayerController : MonoBehaviour
{
    [SerializeField]
    float       _speed = 5.0f;
    bool        _moveKeyPressed = false;
    Animator    _animator;

    float       _movePacketSendTick = 0.2f;
    PlayerInfo  _info = new PlayerInfo();

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

    // Start is called before the first frame update
    void Start()
    { 
        _animator = GetComponent<Animator>();

        StartCoroutine(CoStartMovePacketSend());
    }


    Define.CreatureState _state = Define.CreatureState.Idle;

    public Define.CreatureState State
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
    //float _idle_run_ratio = 0.0f;

    void Update()
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

        switch (_state)
        {
            case Define.CreatureState.Idle:
                UpdateIdle();
                break;
            case Define.CreatureState.Moving:
                UpdateMoving();
                break;
        }
    }

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

    void UpdateIdle()
    {
        //_idle_run_ratio = Mathf.Lerp(_idle_run_ratio, 0.0f, 10.0f * Time.deltaTime);
        //_animator.SetFloat("idle_run_ratio", _idle_run_ratio);
        //_animator.Play("IDLE_RUN");

        //_animator.SetFloat("speed", 0);
    }

    bool _isMoving = false;

    void UpdateMoving()
    {
        Vector3 moveDir = _destPos - transform.position;
        if (moveDir.magnitude < 0.0001f)
        {
            _isMoving = false;
            transform.position = _destPos;

            if (_moveKeyPressed == false)
                State = Define.CreatureState.Idle;
        }
        else
        {
            float moveDist = Mathf.Clamp(_speed * Time.deltaTime, 0, moveDir.magnitude);
            transform.position += moveDir.normalized * moveDist;
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(moveDir), 20 * Time.deltaTime);
        }

        //_idle_run_ratio = Mathf.Lerp(_idle_run_ratio, 1.0f, 10.0f * Time.deltaTime);
        //_animator.SetFloat("idle_run_ratio", _idle_run_ratio);
        //_animator.Play("IDLE_RUN");

        //_animator.SetFloat("speed", _speed);
    }

    Vector3 _destPos;

    void UpdateInput()
    {
        //  local -> world
        //transform.TransformDirection
        //  world -> local
        //transform.InverseTransformDirection

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

        if (_moveKeyPressed && _isMoving == false)
        {
            State = Define.CreatureState.Moving;
            _destPos = destPos;
            _isMoving = true;
        }
    }
}
