using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    [SerializeField]
    Define.CameraMode _mode = Define.CameraMode.TopView;
    [SerializeField]
    Vector3 _delta = new Vector3(0.0f, 8.0f, 0f);

    public GameObject MyPlayer = null;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void LateUpdate()
    {
        if (MyPlayer && _mode == Define.CameraMode.TopView)
        {
            RaycastHit hit;
            //  카메라와 나 사이에 먼가 있어서 시야가 가린다.
            if (Physics.Raycast(MyPlayer.transform.position, _delta, out hit, _delta.magnitude, LayerMask.GetMask("Wall")))
            {
                float dist = (hit.point - MyPlayer.transform.position).magnitude * 0.8f;
                transform.position = MyPlayer.transform.position + _delta.normalized * dist;
            }
            else
            {
                transform.position = MyPlayer.transform.position + _delta;
                transform.LookAt(MyPlayer.transform);
            }
        }
    }

    public void SetTopView(Vector3 delta)
    {
        _mode = Define.CameraMode.TopView;
        _delta = delta;
    }
}
