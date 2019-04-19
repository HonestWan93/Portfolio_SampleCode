using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
public class Lockermanager : MonoBehaviour
{
    private List<ButtonLocker> m_lLockers = new List<ButtonLocker>();

    private ButtonLocker Temp;
    private ElevatorGame m_elevator;

    public GameObject m_GobPasswordObj;
    public UILabel m_labelPasswordText;

    public int m_nNumber = 0;

    private float m_fFPS = 0.02f;
    public float OpenAngle = 45f;           // 금고 열기 최대각도                              

    private string m_strUIString = null;    // UI에 띄우는 글자
    public string m_LockerNumber = null;    // 입력받은 번호

    public bool m_bNumberInsert = false;
    public bool StageEnable = true;
    public bool m_bHandleRotate = false;


    private WaitForSeconds m_wairFPS = new WaitForSeconds(0.02f);

    private static Lockermanager m_LockerManagerInstance = null;

    //locker매니저 싱글톤
    public static Lockermanager LockerManger
    {
        get
        {
            if (m_LockerManagerInstance == null)
            {
                m_LockerManagerInstance = FindObjectOfType(typeof(Lockermanager)) as Lockermanager;
                if (m_LockerManagerInstance == null)
                {
                    print("Locker메니저가 없음");
                }
            }

            return m_LockerManagerInstance;
        }
    }   

    void Awake()
    {
        m_labelPasswordText = GameObject.Find("PasswordText").GetComponent<UILabel>();
        m_labelPasswordText.text = m_strUIString;

        m_GobPasswordObj = GameObject.Find("PasswordUI");
        m_GobPasswordObj.SetActive(false);
        DontDestroyOnLoad(this);
    }

    public ElevatorGame SetElevator(ElevatorGame _elevator)
    {
        m_elevator = _elevator;
        return m_elevator;
    }

    public ElevatorGame GetElevator()
    {
        return m_elevator;
    }  

    //UI에 글자 null 처리 함수
    public void NullUIstringFun()
    {
        m_strUIString = null;
        m_labelPasswordText.text = m_strUIString;
    }
    
    //입력한 번호 넣는 함수
    public void InsertLockerNumber(string _strLockerNumber)
    {
        Temp.m_ButtonPress = m_bNumberInsert;
        m_LockerNumber = _strLockerNumber;
        m_nNumber = m_LockerNumber.IndexOf(" ");

        if (m_nNumber == -1)
        {
            m_nNumber = m_LockerNumber.IndexOf("_");
            m_LockerNumber = m_LockerNumber.Substring(m_nNumber + 1);
            if (m_LockerNumber.Equals("close"))
            {
                StartCoroutine(Temp.ButtonRestCorutin());
                m_strUIString = null;
            }
            if (m_LockerNumber.Equals("open"))
            {
                Temp.OpenNumberLockerFun();
                m_bHandleRotate = Temp.m_bHandleRoatating;
                m_strUIString = null;
                m_GobPasswordObj.SetActive(false);
            }
            m_labelPasswordText.text = m_strUIString;
            Temp.m_strButtonOC = m_LockerNumber;

        }
        else
        {
            StartCoroutine(PasswordCorutin());
            m_LockerNumber = m_LockerNumber.Substring(m_nNumber + 1);
            m_nNumber = int.Parse(m_LockerNumber);
            Temp.m_nButtonNumber = m_nNumber;

        }


    }

    // 컷씬 넘어 갈때 마다 금고 얻어오는 함수
    public IEnumerator LockerGetCorutin()
    {
        m_lLockers.Clear();
        yield return null;

        for (int i = 0; i < GameObject.FindGameObjectsWithTag("Locker").Length; i++)
        {
            if (GameObject.FindGameObjectsWithTag("Locker")[i].GetComponent<ButtonLocker>() != null)
            {
                m_lLockers.Add(GameObject.FindGameObjectsWithTag("Locker")[i].GetComponent<ButtonLocker>());

            }

        }
    }

    //UI 이미지 띄우기
    IEnumerator PasswordCorutin()
    {
        yield return null;

        if (m_strUIString == null)
        {
            m_strUIString = "*";
        }
        else if (m_strUIString.Equals("*"))
        {
            m_strUIString = "**";
        }
        else if (m_strUIString.Equals("**"))
        {
            m_strUIString = "***";
        }
        else if (m_strUIString.Equals("***"))
        {
            m_strUIString = "****";
        }

        yield return null;
        m_labelPasswordText.text = m_strUIString;
    }

    //금고함 열기
    public void OpenLocker(Collider _colLocker)
    {
        if (Temp.m_LockerOpne == true)
        {
            if (StateMangerScript.StateManger.m_GameState == StateMangerScript.StateGameMode.LockModeState &&
                StateMangerScript.StateManger.m_GameStageState != StateMangerScript.StageStateMode.TimeStateMode)
            {
                StateMangerScript.StateManger.m_GameState = StateMangerScript.StateGameMode.GamePalyState;
            }

            _colLocker.enabled = false;
            Temp.m_LockerOpne = false;
            m_bHandleRotate = false;

            StartCoroutine(LOpenBreakDoorCorutin(-90, Temp.m_cParentCol, -4.0f));
        }
    }

    //금고함 찾기
    public void FindLocker(string _str)
    {
        for (int i = 0; i < m_lLockers.Count; i++)
        {
            if (m_lLockers[i] != null)
            {
                if (_str == m_lLockers[i].gameObject.name)
                {

                    StartCoroutine(m_lLockers[i].ButtonRestCorutin());
                    StartCoroutine(m_lLockers[i].NumberCorutin());
                    Temp = m_lLockers[i];
                    break;
                }
            }
        }
    }


    // 왼쪽 문 여는 코루틴 (연속 누름 방지)
    public IEnumerator LOpenBreakDoorCorutin(float _fMaxAngle, Collider _gobTarget, float _fRotationSpeed)
    {

        if (_gobTarget != null)
        {
            _gobTarget.enabled = false;
            yield return null;
            bool bOpneDoor = true;

            float fLerpTime = 0.0f;

            float fLerpAngle = 180 - _gobTarget.transform.localEulerAngles.y;


            while (fLerpTime <= 1.0f && fLerpAngle != _fMaxAngle)
            {
                yield return null;
                fLerpTime += Time.deltaTime;
                fLerpAngle = Mathf.Lerp(0.0f, _fMaxAngle, fLerpTime);
                _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
                bOpneDoor = false;
            }

            while (fLerpTime <= 1.0f && bOpneDoor == true)
            {
                yield return null;
                fLerpTime += Time.deltaTime;
                fLerpAngle = Mathf.Lerp(_fMaxAngle, 0.0f, fLerpTime);
                _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
            }

            _gobTarget.enabled = true;
        }
    }



    //오른쪽 문 여는 코루틴 (연속 누름 방지)
    public IEnumerator ROpenBreakDoorCorutin(float _fMaxAngle, Collider _gobTarget, float _fRotationSpeed)
    {
        _gobTarget.enabled = false;
        yield return null;
        bool bOpneDoor = false;

        float fLerpTime = 0.0f;
        float fLerpAngle = _gobTarget.transform.localEulerAngles.y;

        while (fLerpTime <= 1.0f && fLerpAngle != _fMaxAngle)//문을 열때
        {
            yield return null;
            fLerpTime += Time.deltaTime;
            fLerpAngle = Mathf.Lerp(0.0f, _fMaxAngle, fLerpTime);
            _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
            bOpneDoor = true;
        }

        while (fLerpTime <= 1.0f && bOpneDoor == false)//문을 닫을때
        {
            yield return null;
            fLerpTime += Time.deltaTime;
            fLerpAngle = Mathf.Lerp(_fMaxAngle, 0.0f, fLerpTime);
            _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
        }

        _gobTarget.enabled = true;
    }

    // 가방 케이스(위로 열리는) 여는 코루틴
    public IEnumerator UpOpendoorCorutin(float m_fMaxAngle, GameObject _gobTarget, float _fRotationSpeed)
    {
        float fCaseAngle = 180 - _gobTarget.transform.localEulerAngles.x;
        _gobTarget.transform.localEulerAngles = new Vector3(_gobTarget.transform.localEulerAngles.x, 0.0f, 0.0f);
        while (_gobTarget.transform.localEulerAngles.x > m_fMaxAngle || fCaseAngle == 0 || fCaseAngle == 180)
        {
            fCaseAngle = 180 - _gobTarget.transform.localEulerAngles.x;
            _fRotationSpeed += _fRotationSpeed * m_fFPS;
            _gobTarget.transform.Rotate(_fRotationSpeed, 0.0f, 0.0f);

            if (_gobTarget.transform.localEulerAngles.y == 180 || _gobTarget.transform.localEulerAngles.z == 180)
            {
                break;
            }
            yield return m_wairFPS;
        }

    }

    // 서랍장 여는 코루틴 (연속 누름 방지)
    public IEnumerator OpendBreakDeskdrawCorutin(Collider _colTarget, float _fMoveSpeed)
    {
        _colTarget.enabled = false;
        bool bOpneDeskDraw = false;
        SoundManagerScript.SoundInstance.Play(9);
        float fLerpTime = 0.0f;

        Vector3 _v3TempLocalPos = _colTarget.transform.localPosition;
        while (fLerpTime <= 1.0f && _v3TempLocalPos.z != 0.5f)
        {

            fLerpTime += Time.deltaTime * 2.0f;
            _colTarget.transform.localPosition = Vector3.Lerp(_v3TempLocalPos,
                new Vector3(_colTarget.transform.localPosition.x, _colTarget.transform.localPosition.y,
               0.5f), fLerpTime);
            bOpneDeskDraw = true;
            yield return null;

        }

        while (fLerpTime <= 1.0f && bOpneDeskDraw == false)
        {

            fLerpTime += Time.deltaTime;
            _colTarget.transform.localPosition = Vector3.Lerp(_colTarget.transform.localPosition,
                    new Vector3(_colTarget.transform.localPosition.x, _colTarget.transform.localPosition.y,
                   0.091f), fLerpTime);
            yield return null;
        }

        _colTarget.enabled = true;

    }

    //ABC 퍼즐 풀렸을때 열리는 코루틴
    public IEnumerator ROpenABCDoorCorutin(float _fMaxAngle, GameObject _gobTarget, float _fRotationSpeed, IEnumerator PuzzleOpenDoorCorutin)
    {
        Collider Door = _gobTarget.GetComponent<Collider>();
        Door.enabled = false;
        yield return new WaitForSeconds(4.0f);
        CameraManagerScript.CameraManger.GetPuzzleCamera().enabled = false;

        float fLerpTime = 0.0f;
        float fLerpAngle = _gobTarget.transform.localEulerAngles.y;
        SoundManagerScript.SoundInstance.Play(18);

        while (fLerpTime <= 1.0f && fLerpAngle != _fMaxAngle)//열리는 거
        {
            yield return null;
            fLerpTime += Time.deltaTime;
            fLerpAngle = Mathf.Lerp(0.0f, _fMaxAngle, fLerpTime);
            _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
        }

        StartCoroutine(PuzzleOpenDoorCorutin);
    }

    //ABC 퍼즐 풀렸을때 시네마틱 효과에 사용되는 코루틴
    public IEnumerator ROpenABCDoorCorutin2(float _fMaxAngle, GameObject _gobTarget, float _fRotationSpeed, IEnumerator CineMaticEndCorutin)
    {
        Collider Door = _gobTarget.GetComponent<Collider>();
        Door.enabled = true;
        //bool bOpneDoor = false;

        float fLerpTime = 0.0f;
        float fLerpAngle = _gobTarget.transform.localEulerAngles.y;
        SoundManagerScript.SoundInstance.Play(18);


        while (fLerpTime <= 1.0f)//닫히는 거
        {
            yield return null;
            fLerpTime += Time.deltaTime;
            fLerpAngle = Mathf.Lerp(_fMaxAngle, 0.0f, fLerpTime);
            _gobTarget.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.up);
        }

        StartCoroutine(CineMaticEndCorutin);

    }

}
