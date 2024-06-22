using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Managers : MonoBehaviour
{
    static Managers s_instance;
    static Managers Instance { get { Initialize();  return s_instance; } }

    ObjectManager _object = new ObjectManager();
    public static ObjectManager Object { get { return Instance._object; } }


    NetworkManager _network = new NetworkManager();
    DataManager _data = new DataManager();
    InputManager _input = new InputManager();
    PoolManager _pool = new PoolManager();
    ResourceManager _resource = new ResourceManager();
    UIManager   _ui = new UIManager();
    SceneManagerEx   _scene = new SceneManagerEx();
    SoundManager   _sound = new SoundManager();

    public static NetworkManager Network { get { return Instance._network; } }
    public static DataManager Data { get { return Instance._data; } }
    public static InputManager Input { get { return Instance._input; } }
    public static PoolManager Pool { get { return Instance._pool; } }
    public static ResourceManager Resource { get { return Instance._resource; } }
    public static UIManager UI { get { return Instance._ui; } } 
    public static SceneManagerEx Scene { get { return Instance._scene; } } 
    public static SoundManager Sound { get { return Instance._sound; } } 

    void Start()
    {
        Initialize();
    }

    void Update()
    {
        _input.OnUpdate();
        _network.Update();
    }

    static void Initialize()
    {
        if(s_instance == null)
        {
            GameObject go = GameObject.Find("@Managers");
            if(go == null)
            {
                go = new GameObject { name = "@Managers" };
                go.AddComponent<Managers>(); 
            }

            DontDestroyOnLoad(go);
            s_instance = go.GetComponent<Managers>();

            s_instance._data.Initialize();
            s_instance._pool.Initialize();
            s_instance._sound.Initialize();
        }
    }

    public static void Clear()
    {
        Sound.Clear();
        Input.Clear();
        UI.Clear();
        Scene.Clear();
        Pool.Clear();
    }
}