using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System;
using System.IO;
using UnityEngine;



public class InventoryControllerScript : MonoBehaviour
{ 
    private List<ItemScript> m_lItems = new List<ItemScript>();
    private List<GameObject> ItemObj = new List<GameObject>();

    public GameObject m_gObjSampleItem;
    public GameObject m_gobView;
    public GameObject m_gobPannelBack;

    public UIScrollView m_scrollView;
    public UIGrid m_grid;

    public Vector3 m_v3SaveObjectPos;

    public Quaternion m_qSaveObjectQuater;
    public Quaternion m_qSaveObjectYQuater;

    void Awake()
    {

        m_gobPannelBack = GameObject.Find("Inventory_Panel_Back");
      
        for (int i = 0; i < GameObject.FindGameObjectsWithTag("Item").Length; i++)
        {
            ItemObj.Add(GameObject.FindGameObjectsWithTag("Item")[i]);

        }
        m_grid.gameObject.SetActive(false);

    }

    public void DeleteItemBarlist()
    {
        for (int i = 0; i < m_lItems.Count;)//아이템 지우기
        {
            if (m_lItems[i].m_Item.NextSaveItem.Equals("Zero"))//제로 일 때 사라지기
            {
                DeleteItemBar(m_lItems[i]);
                i = 0;
            }
            else if (m_lItems[i].m_Item.NextSaveItem.Equals("One"))
            {
                ++i;
            }
            else if (m_lItems[i].m_Item.NextSaveItem.Equals("Two"))
            {
                ++i;
            }
        }
    }

    public void ItemClearFunc()
    {
        DeleteItemBarlist();

        for (int i = 0; i < m_lItems.Count; ++i)//카운트 변화 주기
        {
            if (m_lItems[i].m_Item.NextSaveItem.Equals("One"))//아이템을 보유할 수 있는 스테이지 카운트
            {
                m_lItems[i].SetObj(GameObject.Find(m_lItems[i].m_Item.Name));
                m_lItems[i].GetItemView().SetActive(false);

                m_lItems[i].m_Item.NextSaveItem = "Zero";
            }
            else if (m_lItems[i].m_Item.NextSaveItem.Equals("Two"))//아이템을 보유할 수 있는 스테이지 카운트
            {
                m_lItems[i].m_Item.NextSaveItem = "One";
            }
        }

        ItemObj.Clear();

        for (int i = 0; i < GameObject.FindGameObjectsWithTag("Item").Length; i++)
        {
            ItemObj.Add(GameObject.FindGameObjectsWithTag("Item")[i]);
        }
    }

    public List<ItemScript> GetItemListFun()
    {
        return m_lItems;
    }


    // 키를 누르면 아이템을 화면에 보여주는 역할을 함
    public GameObject SetItemObj(GameObject _gobItem)
    {
        m_gobView = _gobItem;
        return m_gobView;
    }


    //경로 읽어 들일때
    public void AddItemButton(string _strItemName)
    {
        //쓸모 없는 문자를 잘라준다. (UnityEngine) <- 이거 자른 거
        _strItemName = _strItemName.Remove(_strItemName.IndexOf(" "));

        //i가 1인 이유 아이템리스트에서 2번째 값부터 읽어드리는걸 실행할려고 +1을 붙이는 이유는 마지막 값을 읽기 위해서이다
        for (int i = 1; i < ItemManager.ItemInstance.GetItemsCount() + 1; i++)
        {
            if (ItemManager.ItemInstance.GetItem(i).Name.Equals(_strItemName))
            {
                ItemInfo newItemInfo = ItemManager.ItemInstance.GetItem(i);
                if (newItemInfo != null)
                {
                    this.AddItem(newItemInfo);
                }
            }
        }
    }


    public void AddItem(ItemInfo _itemInfo)
    {
        if (_itemInfo.HaveItem.Equals("o"))
        {
            GameObject newItemBarObject = Instantiate(this.m_gObjSampleItem, this.transform.position, Quaternion.identity) as GameObject;

            //새로 생성된 아이템바의 부모는 나 자신이다.
            newItemBarObject.transform.SetParent(this.m_grid.transform);

            //스케일 초기화
            newItemBarObject.transform.localScale = new Vector3(0.6f, 0.6f, 0.6f);

            //만들어진 게임오브젝트에서 
            //아이템바 컴포넌트 인스턴스를 얻어 아이템 인포셋팅
            ItemScript Item = newItemBarObject.GetComponent<ItemScript>();
            Item.SetInfo(_itemInfo);

            m_lItems.Add(Item);

            if (m_gobView != null)
            {
                for (int i = 0; i < m_lItems.Count; i++)
                {
                    if (m_lItems[i].m_Item.Name == m_gobView.name)
                    {
                        if (m_gobView.activeSelf == true)
                        {
                            Item.SetObj(m_gobView);
                            m_gobView.SetActive(false);
                        }
                    }

                }
            }
        }
        else if (_itemInfo.HaveItem.Equals("x"))
        {
            if (_itemInfo.ObjectState.Equals("R"))
            {
                m_v3SaveObjectPos = m_gobView.transform.position;
                m_qSaveObjectQuater = m_gobView.transform.rotation;
                m_qSaveObjectYQuater = m_gobView.transform.GetChild(0).transform.rotation;
                StateMangerScript.StateManger.m_GameState = StateMangerScript.StateGameMode.ObjectRotateState;
                m_gobView.transform.position =CameraManagerScript.CameraManger.GetMainCamera().transform.position
                     + CameraManagerScript.CameraManger.GetMainCamera().transform.forward * 0.5f;
                m_gobView.transform.LookAt(CameraManagerScript.CameraManger.GetMainCamera().transform);
            }
        }

        //그리드 정렬
        m_grid.Reposition();
        m_scrollView.ResetPosition();
    }




    // 모두 삭제
    public void ClearAll()
    {
        // 아까 만든거 저장해둔 리스트에서 모두 삭제
        for (int nIndex = 0; nIndex < m_lItems.Count; nIndex++)
        {
            if (m_lItems[nIndex] != null && m_lItems[nIndex].gameObject != null)
            {
                Destroy(m_lItems[nIndex].gameObject);
            }

        }

        // 삭제
        m_lItems.Clear();

        // 그리드 재정렬 
        m_grid.Reposition();
        m_scrollView.ResetPosition();
    }

    public void DeleteItemBar(ItemScript itemBar)
    {
        m_lItems.Remove(itemBar);
        Destroy(itemBar.gameObject);
        StartCoroutine(CoDeleteAfterReposition());
    }

    IEnumerator CoDeleteAfterReposition()
    {
        yield return null;
        this.m_grid.Reposition();
    }


    //리스트에 저장되어있는 ItemID로 저장
    public void SaveHaveItemID()
    {
        XmlWriter xmlWriter;

        //파일경로
        string filePath =
            Application.persistentDataPath + "/Myitem.xml";

        try
        {
            XmlWriterSettings xmlSetting = new XmlWriterSettings();
            xmlSetting.Indent = true;
            xmlSetting.Encoding = Encoding.UTF8;

            xmlWriter = XmlWriter.Create(
                filePath,
                xmlSetting
                );
        }
        catch (Exception e)
        {
            print(e.Message);
            return;
        }

        xmlWriter.WriteStartElement("MyItem");

        int childItemBarCount = this.m_grid.transform.childCount;

        for (int i = 0; i < childItemBarCount; i++)
        {
            //작은 번호가 하이어라키
            Transform childTrans = this.m_grid.transform.GetChild(i);
            ItemScript itemBar =
                childTrans.gameObject.GetComponent<ItemScript>();

            if (itemBar != null)
            {
                int itemId = itemBar.GetInfo().Id;
                string strItemName = itemBar.GetInfo().Name;
                string strIcon = itemBar.GetInfo().Icon;
                string strHaveItem = itemBar.GetInfo().HaveItem;
                string strNextSaveItem = itemBar.GetInfo().NextSaveItem;
                string strObjectState = itemBar.GetInfo().ObjectState;
                string strEventItem = itemBar.GetInfo().EventItem;
                string strRelationObject = itemBar.GetInfo().RelationObject;
                string strLocking = itemBar.GetInfo().Locking;

                xmlWriter.WriteStartElement("Field");

                xmlWriter.WriteAttributeString("id", itemId.ToString());
                xmlWriter.WriteAttributeString("Name", strItemName.ToString());
                xmlWriter.WriteAttributeString("Icon", strIcon.ToString());
                xmlWriter.WriteAttributeString("HaveItem", strHaveItem.ToString());
                xmlWriter.WriteAttributeString("NextSaveItem", strNextSaveItem.ToString());
                xmlWriter.WriteAttributeString("ObjectState", strObjectState.ToString());
                xmlWriter.WriteAttributeString("EventItem", strEventItem.ToString());
                xmlWriter.WriteAttributeString("RelationObject", strRelationObject.ToString());
                xmlWriter.WriteAttributeString("Locking", strLocking.ToString());




                xmlWriter.WriteEndElement();

            }

        }
        xmlWriter.WriteEndElement();
        xmlWriter.Close();
    }

    public void LoadHaveItemID()
    {
       

        XmlReader xmlReader = null;
        string filePath =
            Application.persistentDataPath + "/Myitem.xml";

        try
        {
            XmlReaderSettings xmlSetting = new XmlReaderSettings();
            xmlSetting.IgnoreComments = true;//주석생략
            xmlSetting.IgnoreWhitespace = true;//화이트 스페이스 생략
            xmlReader = XmlReader.Create(filePath, xmlSetting);
        }
        catch (Exception e)
        {
            print(e.Message);
        }

        while (xmlReader.Read())
        {
            if ((xmlReader.Name == "Field") &&
                xmlReader.NodeType == XmlNodeType.Element)
            {
                int itemId = int.Parse(xmlReader.GetAttribute("id"));

                ItemInfo loadItemInfo = ItemManager.ItemInstance.GetItem(itemId);

                if (loadItemInfo != null)
                {
                    this.AddItem(loadItemInfo);
                }

            }
        }
        xmlReader.Close();
    }

}
