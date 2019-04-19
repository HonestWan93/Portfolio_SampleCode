using UnityEngine;
using System.Collections;

public class EnemyBossMoveControl : MonoBehaviour {

    private enum EnemyState                                                                                 //상태(공격,방어,아이델) 
    {
        Attack,
        Defense,
        Idel
    }

    private struct Percentage                                                                               //확률 구조체
    {
        public bool OnOff;                                                                                  //실행을 했는지 안했는지 확인하기 위한 불값 
        public bool EntryExit;                                                                              //확률을 맞추기 위한 불값
        public EnemyState enmystate;                                                                        //상태값
    }

    private Percentage[] EnemyStatePercentage =                                                             //퍼센테이지 4개 할당
      new Percentage[4];

    private Vector3 EnemyHumanDirection = Vector3.zero;                                                     //바라볼 방향
    private GameObject targetPlayer;                                                                        //타겟 설정       
    public GameObject Shield;

    private Animator CharacterAnim;                                                                         //캐릭터 애니메이션
    private AnimatorStateInfo CharacterAnimState;                                                           //캐릭터 Layer
    private Coroutine AnimationCoroutine;                                                                   //코루틴을 담아둘 변수
                                                                                                            // public SphereCollider ShieldColider;
                                                                                                            // public GameObject Shield;

    private float EnemyHumanDistance = 0.0f;                                                                //플레이어의 거리
    private float EnemyHumanSwordAttackDistance = 6.0f;                                                     //근접 공격 거리
    private float MoveBackTimer = 0;                                                                        //맞았을때 뒤로 밀리는 타이머
    private float Timer = 0.0f;                                                                             //속도 패턴 타이머
    private float moveSpeed = 0.0f;                                                                         //적의 움직임 속도

    private bool TriggerEvent = false;
    private bool FirstAttack = false;                                                                       //먼저 공격을 할 것인지
    private bool OnOff = true;                                                                              //코루틴 함수를 한번만 실행하기 위한 변수
    private bool getHit = false;                                                                            //피격 판정시 사용되는 불값
    private bool EnemyCorutine = true;

    private int SpeedCount = 0;                                                                             //속도 카운트
    private int SpeedCountLimit = 2;                                                                        //속도 카운트 제한
    private int AllCount = 0;                                                                               //확률값 초기화를 위한 int
    public int BossHpNum = 10;

    SphereCollider ShieldColider;
    SphereCollider BossCollider;

    GameObject EnemyBoss;
    GameObject BossExplosion;
    BoxCollider EnemySword;

    private int AttackHash = 0;
    // Use this for initialization
    void Awake()
    {
        this.CharacterAnim = this.GetComponent<Animator>();                                                 //애니메이터 가져오기
        this.targetPlayer = GameObject.Find("Player");                                                      //플레이어 타겟 설정
        ShieldColider = GameObject.Find("Shiled").GetComponent<SphereCollider>();
        BossCollider=GameObject.Find("EnemyHuman").GetComponent<SphereCollider>();
        Shield.gameObject.SetActive(false);
        EnemyBoss = this.transform.Find("EnemyHuman").gameObject;
        BossExplosion = this.transform.Find("SmallExplosion").gameObject;
        BossExplosion.SetActive(false);
        EnemySword = GameObject.Find("EnemySwordCollider").GetComponent<BoxCollider>();
        //퍼센테이지 설정
        AttackHash = Animator.StringToHash("EnemyAni.skillMotion1");
        //공격 2 방어 1 아이델 1

        EnemyStatePercentage[0].enmystate =
          EnemyState.Defense;
        EnemyStatePercentage[0].OnOff = false;
        EnemyStatePercentage[0].EntryExit = false;

        EnemyStatePercentage[1].enmystate =
       EnemyState.Idel;
        EnemyStatePercentage[1].OnOff = false;
        EnemyStatePercentage[1].EntryExit = false;

        EnemyStatePercentage[2].enmystate =
          EnemyState.Attack;
        EnemyStatePercentage[2].OnOff = false;
        EnemyStatePercentage[2].EntryExit = false;

        EnemyStatePercentage[3].enmystate =
            EnemyState.Attack;
        EnemyStatePercentage[3].OnOff = false;
        EnemyStatePercentage[3].EntryExit = false;
        BossHpNum = 10;
        //this.gameObject.SetActive(false);
    }
	
	// Update is called once per frame
	void Update ()
    {
        if (TriggerEvent == true)
        {
            if (this.EnemyCorutine == true)
            {
                AnimationCoroutine = StartCoroutine(AnimationIdelPlayEnemy());                              //아이델 코루틴 함수 실행
                EnemyCorutine = false;
            }
        }


        if (OnOff == false && targetPlayer.activeSelf == true)                                                 //코루틴 함수 OnIOff
        {
            OnOff = true;
            AnimationCoroutine = StartCoroutine(EenmyAccessPlayer());
        }

        Timer += Time.deltaTime;
        EnemyHumanDirection = targetPlayer.transform.position- this.transform.position;                     //플레이어를 바라볼 방향
        Vector3.Normalize(EnemyHumanDirection);                                                             //방향 노멀라이즈

        this.transform.rotation = Quaternion.LookRotation(EnemyHumanDirection, Vector3.up);                 //위치 회전

        EnemyHumanDistance = Vector3.Distance(this.transform.position, targetPlayer.transform.position);    //플레이와의 거리

        this.transform.position =
                    Vector3.Lerp(this.transform.position,
                               targetPlayer.transform.position,
                               Time.deltaTime * this.moveSpeed);
        if(BossHpNum<=0)
        {
            this.BossExplosion.SetActive(true);
            this.EnemyBoss.SetActive(false);
            StartCoroutine(BossDie());
           
        }

        if (this.CharacterAnim.GetCurrentAnimatorStateInfo(0).fullPathHash == AttackHash)
        {
            EnemySword.enabled = true;
        }
        else
        {
            EnemySword.enabled = false;
        }

    }


    IEnumerator AnimationIdelPlayEnemy()                                                                    //애니메이션 아이델 플레이 코루틴
    {
        CharacterAnim.SetBool("Attack", false);
        CharacterAnim.SetBool("Defense", false);
        CharacterAnim.SetBool("Move", false);                                                               //아이델 플레이
        this.moveSpeed = 0.0f;                                                                              //움직임은 0
        yield return new WaitForSeconds(1.0f);                                                              //1초간 기다린다.
       
        yield return StartCoroutine(EenmyAccessPlayer());                                                   //접근 코루틴 시작
    }

    IEnumerator EenmyAccessPlayer()
    {
        this.moveSpeed = 0.5f;
        CharacterAnim.SetBool("Move", true);

        while (this.EnemyHumanDistance > EnemyHumanSwordAttackDistance)                                     //접근 공격 거리보다 멀면 붙어라
        {
            this.moveSpeed = 2.0f;
            CharacterAnim.SetBool("Move", true);                                                 
            CharacterAnim.SetBool("Attack", false);
            CharacterAnim.SetBool("Defense", false);
            if (Timer > 3.0F && SpeedCount < SpeedCountLimit)                                               //스피드 증가 카운트
            {
                SpeedCount += 1;
                Timer = 0;
            }
            else if (SpeedCount >= SpeedCountLimit)                                                         //스피드 리미트 보다 같거나 커지면 스피드 증가
            {
                FirstAttack = true;                                                                         //스피드가 증가하면 다음 애니메이션은 공격을 한다
                this.moveSpeed = 2.0f;
            }

            yield return null;
        }

        while (this.EnemyHumanDistance <= EnemyHumanSwordAttackDistance &&
            targetPlayer.activeSelf == true)                                                               //접근 공격 범위 안에 들어 오면 공격
        {
            SpeedCount = 0;                                                                                 //스피드 카운트 0으로 초기화
            this.moveSpeed = 0.0f;                                                           //움직임 0
            CharacterAnim.SetBool("Attack", true);                                                              //desh_front 스탑

            yield return null;

            if (FirstAttack == true)                                                                        //FirstAttack이 true면 바로 공격 코루틴 함수로
            {
                yield return StartCoroutine(AnimationSwordAttackPlayEnemy());                               //공격 코루틴 함수 실행
            }
            else                                                                                            //FirstAttack이 false라면
            {

                int rand = Random.Range(0, 4);                                                              //랜덤으로 확률 숫자 생성
                for (int i = 0; i < 4; i++)
                {
                    if (EnemyStatePercentage[i].OnOff == true && EnemyStatePercentage[i].EntryExit == false)//불값 상태와 AllCount 올려주기
                    {
                        AllCount += 1;
                        EnemyStatePercentage[i].EntryExit = true;
                    }
                }

                if (AllCount >= 4)                                                                          //AllCount, 제안 걸어둔 불값 초기화
                {
                    for (int i = 0; i < 4; i++)
                    {
                        EnemyStatePercentage[i].OnOff = false;
                        EnemyStatePercentage[i].EntryExit = false;
                    }
                    AllCount = 0;
                }

                if (rand == 0)                                                                              //0일때 막기
                {
                    if (EnemyStatePercentage[rand].OnOff == false)                                          //false일때만 들어오기
                    {
                        ShieldColider.enabled = true;
                        BossCollider.enabled = false;
                        this.moveSpeed = 2.0f;
                        CharacterAnim.SetBool("Attack", false);
                        CharacterAnim.SetBool("Move", false);
                        CharacterAnim.SetBool("Defense", true);
                        
                        Shield.SetActive(true);
                        EnemyStatePercentage[rand].OnOff = true;
                        yield return new WaitForSeconds(1.5f);
                    }
                     yield return null;
                }
                else if (rand == 1)                                                                         //1일때 아이델
                {
                    if (EnemyStatePercentage[rand].OnOff == false)
                    {
                        ShieldColider.enabled = false;
                        BossCollider.enabled = true;
                        Shield.SetActive(false);
                        yield return new WaitForSeconds(0.1f);
                        EnemyStatePercentage[rand].OnOff = true;
                        CharacterAnim.SetBool("Attack", false);
                        CharacterAnim.SetBool("Move", false);
                        CharacterAnim.SetBool("Defense", false);
                        yield return new WaitForSeconds(0.5f);
                    }
                    yield return null;
                }
                else                                                                                        //그외 공격 코루틴 함수 실행
                {
                    if (EnemyStatePercentage[rand].OnOff == false)
                    {
                        Shield.SetActive(false);
                        BossCollider.enabled = true;
                        yield return new WaitForSeconds(0.1f);
                        ShieldColider.enabled = false;
                        CharacterAnim.SetBool("Move", false);
                        CharacterAnim.SetBool("Defense", false);
                        EnemyStatePercentage[rand].OnOff = true;
                        StartCoroutine(AnimationSwordAttackPlayEnemy());
                        yield return StartCoroutine(AnimationSwordAttackPlayEnemy());
                    }
                     yield return null;
                }
            }
            if (getHit == true)                                                                             //피격판정이 트루면 코루틴 함수를 나가라
            {
                Shield.SetActive(false);
                ShieldColider.enabled = false;
                yield break;
            }

        }
        OnOff = false;                                                                                      //코루틴 함수 실행 하기위한 불값

    }

    IEnumerator AnimationSwordAttackPlayEnemy()                                                             //공격 코루틴 함수
    {

        CharacterAnim.SetBool("Attack", true);

        yield return new WaitForSeconds(0.5f);

        FirstAttack = false;

        yield return null;
    }

    IEnumerator BossDie()
    {
        yield return new WaitForSeconds(3.0f);
        this.gameObject.SetActive(false);
        yield return null;
    }
    void OnTriggerEnter(Collider col)
    {
        if (col.CompareTag("Detect"))
        {
            this.TriggerEvent = true;
        }

    }
}
