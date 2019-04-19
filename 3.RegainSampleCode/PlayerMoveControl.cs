using UnityEngine;
using System.Collections;

public class PlayerMoveControl : MonoBehaviour
{
    public enum PlayerState                                                                 //상태(아이델,앞,뒤,왼쪽,오른쪽) 
    {
        Idel,
        DeshFront,
        DeshBack,
        DeshLeft,
        DeshRight

    }

    public enum PlayerAttackState
    {
        Sword,
        Gun
    }

    public PlayerState PlayerAnimatonState;                                                 //플레이어 애니메이션 상태값
    public PlayerAttackState PlayerAttackAnimationState;

    private GameObject SpeedLine;
    private GameObject Gun;
    private GameObject Sword;
    private GameObject Bazuca;
    private GameObject Player;

    private Vector3 MousePos;                                                               //마우스 위치 값
    private Vector3 Diagonal;                                                               //대각선 값 설정

    public Transform rootTransform;                                                         //플레이어 위치 값 조정
    public Transform PlayerChildTransform;                                                  //Desh_Back하기 위해 사용
    private GunBulletEntry GunFire;
    Rigidbody isKinematicEnmey;

    private Animator CharacterAnim;                                                         //캐릭터 애니메이션
    private AnimatorStateInfo CharacterAnimState;                                           //캐릭터 Layer

    public float MouseSpeed = 2.0f;                                                         //마우스 감도
    private float MouseMoveX = 0.0f;                                                        //마우스 총 움직임 값
    private float MouseMoveY = 0.0f;                                                        //마우스 총 움직임 값
    private float CharacterMoveSpeed = 20.0f;                                               //플레이어 이동속도

    private bool CursoVisible = false;                                                      //마우스 커서 안보이게 하기위한 불값
    private bool PlayerMoveRimit = false;                                                   //쓸데 없는 연산을 막기 위해 사용
    private bool Desh = false;
    private bool ChangeWeapn = false;

    public bool AccessAttack = false;                                                       //접근 공격하기 위한 불값


    private int BlockkHash = 0;                                                             //애니메이터 공격 state 해쉬 값을 받기 위한 int값
    private int GunHash = 0;
    private int AttackHash = 0;
    private float HorizontalX;
    private float VerticalZ;
    private SphereCollider PlayerCollider;
    private BoxCollider PlayerSword;

    private PlayerSwordAttackScript DeshAccess;

    void Awake()
    {
        SoundManager.SoundInstance.Play(0);
        SoundManager.SoundInstance.loop(0, true);
        this.CharacterAnim = this.GetComponent<Animator>();                                 //애니메이터 가져오기
        this.CharacterAnim.SetLayerWeight(1, 1.0f);                                         //레이어 가중치 적용



        PlayerAnimatonState = PlayerState.Idel;                                             //처음 플레이어 상태 idel
        PlayerAttackAnimationState = PlayerAttackState.Sword;
        BlockkHash = Animator.StringToHash("PlayerLayer.Block");                            //방어 해쉬 값
        GunHash = Animator.StringToHash("MoveAttack.Gun_motion");                           //총 해쉬 값
        AttackHash = Animator.StringToHash("MoveAttack.skillMotion1");
        this.SpeedLine = this.gameObject.transform.Find("SpeedLineParent").gameObject;
        this.Sword = GameObject.Find("Sword");
        this.Gun = this.gameObject.transform.Find("Player").gameObject.transform.           //자식을 이런 방식으로도 찾을 수 있다.
            Find("Parts").gameObject.transform.Find("Gun").gameObject;
        GunFire = GameObject.Find("GunBulletEntry").GetComponent<GunBulletEntry>();
        this.Bazuca = GameObject.Find("WeaponPower");
        this.DeshAccess = this.gameObject.transform.Find("Player").GetComponent<PlayerSwordAttackScript>();
        this.Player = this.gameObject.transform.Find("Player").gameObject;
        this.isKinematicEnmey = this.gameObject.GetComponent<Rigidbody>();
        PlayerCollider = this.gameObject.GetComponent<SphereCollider>();
        PlayerSword = GameObject.Find("SwordCollider").GetComponent<BoxCollider>();
    }



  
    void Update()
    {
        Cursor.visible = false;                                                             //마우스 커서 안보이게
        Cursor.lockState = CursorLockMode.Locked;                                           //마우스 커서 밖으로 못나가게
        this.CharacterAnimState = this.CharacterAnim.GetCurrentAnimatorStateInfo(0);        //레이어 상태 업데이트
        this.Player.transform.localPosition = new Vector3(0, 0, 0);
        //마우스 커서
        #region
        if (Input.GetKeyDown(KeyCode.LeftAlt))                                              //마우스 커서 제한 풀기
        {
            Cursor.lockState = CursorLockMode.Confined;
            CursoVisible = true;
        }

        if (Input.GetMouseButtonDown(0))                                                    //마우스 커서 숨기기
        {
            if (CursoVisible == true)
            {
                Cursor.visible = false;
                Cursor.lockState = CursorLockMode.Locked;
            }
            CursoVisible = false;
        }
        #endregion
        //마우스 회전
        MouseRotate();

        //움직임
        PlayerMove();

        //애니메이션
        PlayerAnimation();

        //공격 방어
        PlayerAttackAndDefence();

        PlayerDesh();
    }

    //자식들 축 설정
    void LateUpdate()
    {
        rootTransform.localPosition = Vector3.zero;
    }

    //플레이어 Control
    void PlayerMove()
    {
        if (this.CharacterAnim.GetCurrentAnimatorStateInfo(0).fullPathHash != BlockkHash && AccessAttack == false)
        {                                                                                   //이동 W,S,A,D
            HorizontalX = Input.GetAxis("Horizontal");
            VerticalZ = Input.GetAxis("Vertical");
            #region
            if (Input.GetKey(KeyCode.W) && PlayerMoveRimit == false)
            {
                PlayerAnimatonState = PlayerState.DeshFront;
                this.transform.Translate(0, 0, CharacterMoveSpeed * Time.deltaTime);
            }
            else if (Input.GetKeyUp(KeyCode.W))
            {
                CharacterAnim.SetBool("Desh_Front", false);
                PlayerIdelMotion();
            }

            if (Input.GetKey(KeyCode.S) && PlayerMoveRimit == false)
            {
                PlayerChildTransform.localRotation = Quaternion.Euler(-10, 0, 0);
                PlayerAnimatonState = PlayerState.DeshBack;
                this.transform.Translate(0, 0, (-CharacterMoveSpeed) * Time.deltaTime);
            }
            else if (Input.GetKeyUp(KeyCode.S))
            {
                CharacterAnim.SetBool("Desh_Back", false);
                PlayerIdelMotion();
            }

            if (Input.GetKey(KeyCode.A) && PlayerMoveRimit == false)
            {
                PlayerAnimatonState = PlayerState.DeshLeft;
                this.transform.Translate((-CharacterMoveSpeed) * Time.deltaTime, 0, 0);
            }
            else if (Input.GetKeyUp(KeyCode.A))
            {

                CharacterAnim.SetBool("Desh_Left", false);
                PlayerIdelMotion();

            }

            if (Input.GetKey(KeyCode.D) && PlayerMoveRimit == false)
            {
                PlayerAnimatonState = PlayerState.DeshRight;
                this.transform.Translate(CharacterMoveSpeed * Time.deltaTime, 0, 0);
            }
            else if (Input.GetKeyUp(KeyCode.D))
            {
                CharacterAnim.SetBool("Desh_Right", false);
                PlayerIdelMotion();
            }

            if (Input.GetKey(KeyCode.LeftShift))
            {
                if (HorizontalX != 0 || VerticalZ != 0)
                {

                    Desh = true;
                    CharacterMoveSpeed = 25;

                }

            }

            if (Input.GetKey(KeyCode.Q) && PlayerMoveRimit == false)
            {
                PlayerAnimatonState = PlayerState.Idel;
                this.transform.Translate(0, -(CharacterMoveSpeed) * Time.deltaTime, 0);
            }
            else if (Input.GetKeyUp(KeyCode.Q))
            {


                PlayerIdelMotion();

            }



            if (Input.GetKey(KeyCode.E) && PlayerMoveRimit == false)
            {
                PlayerAnimatonState = PlayerState.Idel;
                this.transform.Translate(0, (CharacterMoveSpeed) * Time.deltaTime, 0);
            }
            else if (Input.GetKeyUp(KeyCode.E))
            {


                PlayerIdelMotion();

            }

            if (Input.GetKeyDown(KeyCode.LeftShift))
            {

               SoundManager.SoundInstance.Play(7);
                
            }


            #endregion

            //대각선
            #region
            if (Input.GetKey(KeyCode.A) && Input.GetKey(KeyCode.W))
            {
                PlayerMoveRimit = true;
                Diagonal.Set(-CharacterMoveSpeed, 0, CharacterMoveSpeed);
                Diagonal = Diagonal.normalized * CharacterMoveSpeed * Time.deltaTime;
                this.transform.Translate(Diagonal);
            }

            if (Input.GetKey(KeyCode.D) && Input.GetKey(KeyCode.W))
            {
                PlayerMoveRimit = true;
                Diagonal.Set(CharacterMoveSpeed, 0, CharacterMoveSpeed);
                Diagonal = Diagonal.normalized * CharacterMoveSpeed * Time.deltaTime;
                this.transform.Translate(Diagonal);
            }

            if (Input.GetKey(KeyCode.A) && Input.GetKey(KeyCode.S))
            {
                PlayerMoveRimit = true;
                Diagonal.Set(-CharacterMoveSpeed, 0, -CharacterMoveSpeed);
                Diagonal = Diagonal.normalized * CharacterMoveSpeed * Time.deltaTime;
                this.transform.Translate(Diagonal);
            }

            if (Input.GetKey(KeyCode.D) && Input.GetKey(KeyCode.S))
            {
                PlayerMoveRimit = true;
                Diagonal.Set(CharacterMoveSpeed, 0, -CharacterMoveSpeed);
                Diagonal = Diagonal.normalized * CharacterMoveSpeed * Time.deltaTime;
                this.transform.Translate(Diagonal);
            }
            #endregion

            //대각선 예외처리
            #region
            if (Input.GetKey(KeyCode.A) && Input.GetKeyUp(KeyCode.W) ||
                 Input.GetKeyUp(KeyCode.A) && Input.GetKey(KeyCode.W) ||
                 Input.GetKey(KeyCode.D) && Input.GetKeyUp(KeyCode.W) ||
                 Input.GetKeyUp(KeyCode.D) && Input.GetKey(KeyCode.W) ||
                 Input.GetKey(KeyCode.A) && Input.GetKeyUp(KeyCode.S) ||
                 Input.GetKeyUp(KeyCode.A) && Input.GetKey(KeyCode.S) ||
                 Input.GetKey(KeyCode.D) && Input.GetKeyUp(KeyCode.S) ||
                 Input.GetKeyUp(KeyCode.D) && Input.GetKey(KeyCode.S))
            {
                PlayerMoveRimit = false;
            }
            #endregion
        }
        if (Input.GetKeyUp(KeyCode.LeftShift))
        {
            CharacterMoveSpeed = 20;
            Desh = false;

        }
    }

    //마우스 회전 값
    void MouseRotate()
    {
        // if (AccessAttack == false)
        {
            MouseMoveX = MouseSpeed * Input.GetAxis("Mouse X");                                 //마우스 축 값 가져오기
            MousePos = new Vector3(0, MouseMoveX, 0);                                           //마우스 값 받기
            this.transform.Rotate(MousePos);
        }
    }

    //플레이어 애니메이션
    void PlayerAnimation()
    {
        if (PlayerAnimatonState == PlayerState.DeshFront)
        {
            CharacterAnim.SetBool("Desh_Front", true);
            CharacterAnim.SetBool("Desh_Left", false);
            CharacterAnim.SetBool("Desh_Right", false);
            CharacterAnim.SetBool("Desh_Back", false);
        }
        else if (PlayerAnimatonState == PlayerState.DeshLeft)
        {
            CharacterAnim.SetBool("Desh_Back", false);
            CharacterAnim.SetBool("Desh_Front", false);
            CharacterAnim.SetBool("Desh_Right", false);
            CharacterAnim.SetBool("Desh_Left", true);
        }
        else if (PlayerAnimatonState == PlayerState.DeshRight)
        {
            CharacterAnim.SetBool("Desh_Back", false);
            CharacterAnim.SetBool("Desh_Front", false);
            CharacterAnim.SetBool("Desh_Left", false);
            CharacterAnim.SetBool("Desh_Right", true);
        }
        else if (PlayerAnimatonState == PlayerState.DeshBack)
        {
            CharacterAnim.SetBool("Desh_Front", false);
            CharacterAnim.SetBool("Desh_Left", false);
            CharacterAnim.SetBool("Desh_Right", false);
            CharacterAnim.SetBool("Desh_Back", true);
        }
        else if (PlayerAnimatonState == PlayerState.Idel)
        {
            CharacterAnim.SetBool("Desh_Back", false);
            CharacterAnim.SetBool("Desh_Front", false);
            CharacterAnim.SetBool("Desh_Right", false);
            CharacterAnim.SetBool("Desh_Left", false);

            this.PlayerChildTransform.localRotation = Quaternion.Slerp(this.PlayerChildTransform.localRotation,
                 (Quaternion.identity), Time.deltaTime * 2.0f);
        }
    }

    //플레이어 idel모션
    void PlayerIdelMotion()
    {
        if (CharacterAnim.GetBool("Desh_Front") == false && CharacterAnim.GetBool("Desh_Right") == false
            && CharacterAnim.GetBool("Desh_Left") == false && CharacterAnim.GetBool("Desh_Back") == false && AccessAttack == false)
        {
            PlayerAnimatonState = PlayerState.Idel;
            CharacterAnim.ResetTrigger("Attack");
            CharacterAnim.ResetTrigger("GunAttack");
        }
    }

    //플레이어 공격 방어
    void PlayerAttackAndDefence()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            switch (PlayerAttackAnimationState)
            {
                case PlayerAttackState.Sword:
                    PlayerAttackAnimationState = PlayerAttackState.Gun;
                    this.Gun.SetActive(true);
                    this.Sword.SetActive(false);
                    break;

                case PlayerAttackState.Gun:
                    PlayerAttackAnimationState = PlayerAttackState.Sword;
                    this.Gun.SetActive(false);
                    this.Sword.SetActive(true);
                    break;

            }

        }

        if (Input.GetMouseButtonDown(0))
        {
            if (PlayerAttackAnimationState == PlayerAttackState.Sword)
            {
                if (this.CharacterAnim.GetCurrentAnimatorStateInfo(1).fullPathHash != AttackHash)
                {
                    SoundManager.SoundInstance.Play(4);
                }
                AccessAttack = true;

                CharacterAnim.SetTrigger("Attack");
            }

            if (PlayerAttackAnimationState == PlayerAttackState.Gun)
            {
                if (this.CharacterAnim.GetCurrentAnimatorStateInfo(1).fullPathHash != GunHash)
                {
                    SoundManager.SoundInstance.Play(14);
                    GunFire.GunFireFunc();
                    CharacterAnim.SetTrigger("GunAttack");
                }
            }
        }

        if (this.CharacterAnim.GetCurrentAnimatorStateInfo(1).fullPathHash == AttackHash)
        {
            PlayerSword.enabled = true;
        }
        else
        {
            PlayerSword.enabled = false;
        }

        if (Input.GetKey(KeyCode.Space))
        {
            this.CharacterAnim.SetBool("Block", true);
        }
        else if (Input.GetKeyUp(KeyCode.Space))
        {
            this.CharacterAnim.SetBool("Block", false);
        }
    }

    public void GetFrontDesh()
    {
        CharacterAnim.SetBool("Desh_Front", false);
    }

    void PlayerDesh()
    {

        if (Desh == true && this.DeshAccess.Attack == false)
        {
            if (VerticalZ == 0 && HorizontalX > 0)
            {
                SpeedLine.transform.localRotation = Quaternion.Euler(0, 270, 0);
            }

            if (VerticalZ == 0 && HorizontalX < 0)
            {
                SpeedLine.transform.localRotation = Quaternion.Euler(0, 90, 0);
            }

            if (VerticalZ < 0 && HorizontalX == 0)
            {
                SpeedLine.transform.localRotation = Quaternion.Euler(0, 0, 0);
            }

            if (VerticalZ > 0 && HorizontalX == 0)
            {
                SpeedLine.transform.localRotation = Quaternion.Euler(0, 180, 0);
            }


            SpeedLine.SetActive(true);
        }
        else
        {
            SpeedLine.SetActive(false);
        }
    }

    void OnTriggerEnter(Collider col)
    {
        if (col.CompareTag("EnemyBullet") && PlayerCollider.enabled == true)
        {
            GameObject.Destroy(col.gameObject);
            PlayerHPManger.HPInstance.PlayerHp -= 1;
            PlayerHPManger.HPInstance.playerHpCpunt -= 1;
        }
    }

    void OnCollisionStay(Collision col)
    {
        if (col.collider.CompareTag("EnemyParent"))
        {
            AccessAttack = false;
            isKinematicEnmey.isKinematic = true;
        }

    }
}
