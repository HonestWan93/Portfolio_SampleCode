using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonLocker : MonoBehaviour
{

    private  Collider m_cLockercollider;
    public Collider m_cParentCol;               //부모 콜리더
    public Collider m_cthisCol;                 //자신

    public GameObject m_gobdoorHandle;

    private int[] m_anNumberCheck = new int [4];//정답 확인 배열
    public int[] m_anOpenNumber;                //입력 받은 배열
    public int m_nButtonNumber = -1;            //값 입력 정수

    public bool m_bOpendoor = false;
    public bool m_bLockerOpne = false;
    public bool m_bHandleRoatating=false;
    public bool m_bEndLocker = false;
    public bool m_bButtonPress = false;
    public bool m_bPass = false;

    public string m_strButtonOC;                //button Close Open


    void Awake()
    {
        m_cLockercollider = this.gameObject.GetComponent<Collider>();
        m_cParentCol = this.transform.parent.gameObject.GetComponent<Collider>();
        m_cthisCol = this.GetComponent<Collider>();

        for (int i=0; i<4; ++i)
        {
            m_anNumberCheck[i] = -1;
        }
    }

    public void OpenNumberLockerFun()//정답 체크
    {
        bool bOpenCheck = false;
        int nAnswerEmptyCount = m_anNumberCheck.Length;
        int nAnswerCount = 0;
        if (m_bPass == false)
        {

            //-1이 정답이 안들어 갔다.
            for (int i = 0; i < m_anNumberCheck.Length; i++)
            {

                if (m_anNumberCheck[i] == -1)
                {
                    nAnswerEmptyCount -= 1;
                }
            }

            //입력한 값이 정답 값보다 많거나 작을 때 false
            if (nAnswerEmptyCount > m_anOpenNumber.Length || nAnswerEmptyCount < m_anOpenNumber.Length)
            {
                bOpenCheck = false;
            }
            else if (nAnswerEmptyCount == m_anOpenNumber.Length)
            {
                for (int i = 0; i < m_anOpenNumber.Length; i++)
                {
                    if (m_anNumberCheck[i] == m_anOpenNumber[i])
                    {
                        nAnswerCount += 1;
                    }
                }

                if (nAnswerCount == m_anOpenNumber.Length)
                {
                  
                    bOpenCheck = true;
                }
                else
                {
                    bOpenCheck = false;
                }
            }

            if (bOpenCheck == false)
            {
                StateMangerScript.StateManger.m_GameState = StateMangerScript.StateGameMode.GamePalyState;
                m_cthisCol.enabled = true;
                m_bHandleRoatating = false;
            }
            else
            {
                m_cLockercollider.enabled = false;
                m_cthisCol.enabled = false;
                if (m_cParentCol != null)
                {
                    m_cParentCol.enabled = false;
                }
                StartCoroutine(RotateHandleCoroutine());
            }
        }
        else
        {
            //-1이 정답이 안들어 갔다.
            for (int i = 0; i < m_anNumberCheck.Length; i++)
            {

                if (m_anNumberCheck[i] == -1)
                {
                    nAnswerEmptyCount -= 1;
                }
            }

            //입력한 값이 정답 값보다 많거나 작을 때 false
            if (nAnswerEmptyCount > m_anOpenNumber.Length || nAnswerEmptyCount < m_anOpenNumber.Length)
            {
                bOpenCheck = false;
            }
            else if (nAnswerEmptyCount == m_anOpenNumber.Length)//정답 카운트랑 같을때
            {
                
                bOpenCheck = true;
            }

            if (bOpenCheck == false)
            {
                StateMangerScript.StateManger.m_GameState = StateMangerScript.StateGameMode.GamePalyState;
                m_cthisCol.enabled = true;
                m_bHandleRoatating = false;
                StartCoroutine(ButtonRestCorutin());
            }
            else
            {
                m_cthisCol.enabled = false;
                m_cLockercollider.enabled = false;
                m_cParentCol.enabled = false;
                StartCoroutine(RotateHandleCoroutine());
            }
        }
    }

    //금고 손잡이 애니메이션 코루틴
    public IEnumerator RotateHandleCoroutine()
    {
        SoundManagerScript.SoundInstance.Play(4);
        m_bHandleRoatating = true;

        float fLerpTime = 0.0f;
        float fLerpAngle = 0.0f;
        float fCurrentX = m_gobdoorHandle.transform.localEulerAngles.x;

        while (fLerpTime<=1.0f)
        {

            fLerpTime += Time.deltaTime;
            fLerpAngle = Mathf.Lerp(fCurrentX, 90.0f, fLerpTime);
            m_gobdoorHandle.transform.localRotation = Quaternion.AngleAxis(fLerpAngle, Vector3.right);
            yield return null;
        }

        m_bLockerOpne = true;
        m_bEndLocker = true;
        m_bHandleRoatating = false;
     
    }

    //입력값 넣기 및 상태 확인 코루틴
    public IEnumerator NumberCorutin()
    {
        yield return null;

        if (StateMangerScript.StateManger.m_GameState == StateMangerScript.StateGameMode.LockModeState)
        {
            bool block = true;
            for (int i = 0; i < m_anOpenNumber.Length;)
            {
                yield return null;

                if (StateMangerScript.StateManger.m_GameState != StateMangerScript.StateGameMode.LockModeState)
                {
                    block = false;
                    break;
                }

                if (m_strButtonOC != null)
                {
                    if (m_strButtonOC.Equals("close"))
                    {
                        m_strButtonOC = null;
                        m_bButtonPress = false;
                        break;
                    }
                }

                if (m_bButtonPress == true)
                {
                    //락커 매니저에서 얻어온 값 입력
                    m_anNumberCheck[i] = m_nButtonNumber;
                    i++;
                    m_bButtonPress = false;
                }
            }

            yield return null;

            //LockMode 상태에서 계속 돌린다.
            if (block == true)
            {
                StartCoroutine(NumberCorutin());
                yield return null;
            }
        }
    }

    //값 초기화 코루틴
    public IEnumerator ButtonRestCorutin()
    {
        m_bButtonPress = false;
        m_nButtonNumber = -1;
        for (int i = 0; i < m_anNumberCheck.Length; ++i)
        {
            m_anNumberCheck[i] = -1;
        }
        yield return null;

    }

}