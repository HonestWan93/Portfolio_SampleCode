using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ShapDivisionTouchControlScript : MonoBehaviour {

    private CircleCollider2D Cc2dTouchCollide;
    private bool bStayNodeCollider = false;

    public bool bNodeTouch = false;
    public bool bLineDraw = true;
 
    public bool bLineFirstDraw = false;
    public bool bLineStayDraw = false;
    public bool bEndPosiontSave = false;

    public Vector3 v3NodeStartPosition;
    public Vector3 v3NodeEndPosition;
    public List<Vector2> lv2SavePosition = new List<Vector2>();



    void Awake()
    {
        Cc2dTouchCollide = this.GetComponent<CircleCollider2D>();
        Cc2dTouchCollide.enabled = false;
    }


    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Cc2dTouchCollide.enabled = true;
            bLineDraw = true;
        }

        if (Input.GetMouseButtonUp(0))
        {
            bNodeTouch = false;

            bLineStayDraw = false;
            Cc2dTouchCollide.enabled = false;
            bEndPosiontSave = false;
            bLineDraw = false;
        
      
        }

        this.transform.position = Camera.main.ScreenToWorldPoint(Input.mousePosition);
    }

    void OnTriggerEnter2D(Collider2D _cCollider)
    {
        if (_cCollider.CompareTag("SphereNode"))
        {
  
            bNodeTouch = true;
            if (bNodeTouch == true && bLineStayDraw == false)
            {
                bLineFirstDraw = true;
                bLineStayDraw = true;
                v3NodeStartPosition = _cCollider.transform.position;
            }

            if (bNodeTouch == true && bLineStayDraw == true && bLineFirstDraw == false)
            {
                bEndPosiontSave = true;
                v3NodeEndPosition = _cCollider.transform.position;
                bStayNodeCollider = true;
            }

        }
    }

    void OnTriggerStay2D(Collider2D _cCollider)
    {
        if (_cCollider.CompareTag("SphereNode"))
        {
            if (bStayNodeCollider == true)
            {
                if (bNodeTouch == true && bLineStayDraw == false)
                {

                    bLineFirstDraw = true;
                    bLineStayDraw = true;
                    v3NodeStartPosition = _cCollider.transform.position;
                    bStayNodeCollider = false;
                }

            }
        }

    }


    void OnTriggerExit2D(Collider2D _cCollider)
    {
        if (_cCollider.CompareTag("SphereNode"))
        {
            bNodeTouch = false;
            bEndPosiontSave = false;
         

        }

    }
}
