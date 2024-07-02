using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraController : MonoBehaviour
{ 

    public GameObject   MyPlayer = null;
    private Vector2     _moveDelta = Vector2.zero;
    private float       _distance = 5.0f;
    bool                _cursorLocked = true;

    // Start is called before the first frame update
    void Start()
    {
        //Cursor.lockState = CursorLockMode.Confined;

        _moveDelta.x += Input.GetAxis("Mouse X")/* * mouseSensitivity * Time.deltaTime*/;
        _moveDelta.y -= Input.GetAxis("Mouse Y")/* * mouseSensitivity * Time.deltaTime*/;
        _moveDelta.y = Mathf.Clamp(_moveDelta.y, 15.0f, 90f);  

    }
    private float mouseSensitivity = 400f; //마우스감도

    private void Update()
    {
        //if (Input.GetKey(KeyCode.F1))
        //{
        //    if(_cursorLocked)
        //        Cursor.lockState = CursorLockMode.None;
        //    else
        //        Cursor.lockState = CursorLockMode.Confined;

        //    _cursorLocked = !_cursorLocked;
        //}

        if (MyPlayer)
        {
            if (Input.GetMouseButton(1))
            {
                _moveDelta.x += Input.GetAxis("Mouse X") * mouseSensitivity * Time.deltaTime;
                _moveDelta.y -= Input.GetAxis("Mouse Y") * mouseSensitivity * Time.deltaTime;
                _moveDelta.y = Mathf.Clamp(_moveDelta.y, 15.0f, 90f);
            }

            transform.rotation = Quaternion.Euler(_moveDelta.y, _moveDelta.x, 0);
            Vector3 distance = new Vector3(0.0f, 0.0f, _distance);
            transform.position = MyPlayer.transform.position - transform.rotation * distance;
        }
    }

    // Update is called once per frame
    void LateUpdate()
    {
        if (MyPlayer)
        {
            /*
            RaycastHit hit;
            //  카메라와 나 사이에 먼가 있어서 시야가 가린다.
            if (Physics.Raycast(MyPlayer.transform.position, _delta, out hit, _delta.magnitude, LayerMask.GetMask("Wall")))
            {
                float dist = (hit.point - MyPlayer.transform.position).magnitude * 0.8f;
                transform.position = MyPlayer.transform.position + _delta.normalized * dist;
            }
            */

            //transform.position = MyPlayer.transform.position + _delta;
            //transform.LookAt(MyPlayer.transform);
        }
    }

}
