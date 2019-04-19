using UnityEngine;
using System.Collections;

public class ShapeDivisionDraw : MonoBehaviour
{

    private Vector2 pos1;
    private Vector2 pos2;
    private Vector2 v2;

    private GameObject go;
    public GameObject prefab;

    private ShapDivisionTouchControlScript sdtcsNodeCollider;

    public float objectHeight = 1.0f;

    void Awake()
    {
        sdtcsNodeCollider = GameObject.Find("TouchColliderObject").GetComponent<ShapDivisionTouchControlScript>();

    }

    void Update()
    {
        if (sdtcsNodeCollider.bLineFirstDraw == true)
        {

            pos1 = sdtcsNodeCollider.v3NodeStartPosition;

            pos2 = pos1;
            sdtcsNodeCollider.lv2SavePosition.Add(pos1);

            //퍼블릭으로 물린 프리펩 물린다.
            go = Instantiate(prefab) as GameObject;
            go.transform.position = pos1;

            //y크기 값 변경 및 저장
            Vector3 temp = go.transform.localScale;
            temp.y = 0.0f;
            go.transform.localScale = temp;
            sdtcsNodeCollider.bLineFirstDraw = false;

        }

        if (sdtcsNodeCollider.bLineStayDraw == true)
        {
            //노드에 닿지 않고 마우스를 누르고 있는 상태라면 위치값을 건네준다..
            if (sdtcsNodeCollider.bEndPosiontSave == false && sdtcsNodeCollider.bNodeTouch == false)
            {
                pos2 = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            }

            //노드에 닿으면 위치를 노드에 조정한다.
            if (sdtcsNodeCollider.bEndPosiontSave == true && sdtcsNodeCollider.bNodeTouch == true)
            {
                BoxCollider2D GoCollidedr = this.go.GetComponent<BoxCollider2D>();

                GoCollidedr.enabled = true;

                pos2 = sdtcsNodeCollider.v3NodeEndPosition;
                sdtcsNodeCollider.bLineStayDraw = false;
                sdtcsNodeCollider.bEndPosiontSave = false;

                //일정 크기 보다 작으면 지우기
                if (go.transform.localScale.y < 0.8f)
                {
                    go.SetActive(false);
                    GameObject temp = go;
                    GameObject.Destroy(temp);
                }

            }

            if (pos2 != pos1)
            {
                //마우스 드래그 하는 곳으로 끌기
                v2 = pos2 - pos1;
                go.transform.position = pos1 + (v2) / 2.0f;
                go.transform.localScale = new Vector2(go.transform.localScale.x, v2.magnitude / 0.55f);
                go.transform.rotation = Quaternion.FromToRotation(Vector2.up, v2);
            }
        }

        //노드에 안닿고 노으면 삭제 한다.
        if (sdtcsNodeCollider.bNodeTouch == false && sdtcsNodeCollider.bLineDraw == false && sdtcsNodeCollider.bLineStayDraw == false)
        {
            sdtcsNodeCollider.bLineDraw = true;
            if (go != null)
            {

                go.SetActive(false);
                GameObject temp = go;
                GameObject.Destroy(temp);

            }


        }

        for (int i = 0; i < sdtcsNodeCollider.lv2SavePosition.Count; i++)
        {
            for (int j = 1; j < sdtcsNodeCollider.lv2SavePosition.Count; j++)
            {

                if (i < sdtcsNodeCollider.lv2SavePosition.Count - j)
                {
                    if (sdtcsNodeCollider.lv2SavePosition[i] == sdtcsNodeCollider.lv2SavePosition[i + j])
                    {
                        sdtcsNodeCollider.lv2SavePosition.Remove(sdtcsNodeCollider.lv2SavePosition[i + j]);
                    }
                }


            }
        }
    }
}
