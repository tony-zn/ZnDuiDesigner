#include "StdAfx.h"
#include "SettingsWnd.h"
#include <algorithm>

DuiLib::CDuiString CSettingsWnd::GetSkinFolder()
{
	return g_pMainFrame->GetSkinFolder();
}

DuiLib::CDuiString CSettingsWnd::GetSkinFile()
{
	return _T("SettingsWnd.xml");
}

LPCTSTR CSettingsWnd::GetWindowClassName( void ) const
{
	return _T("SettingsWnd");
}

void CSettingsWnd::InitWindow()
{
	__super::InitWindow();
	DUI_GET_CONTROL_PTR(DuiLib::CTabLayoutUI		, __m_pRootTab						, _T("tab"));
	DUI_GET_CONTROL_PTR(DuiLib::CTreeViewUI			, __m_pControlsTree					, _T("controls.tree"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyle				, _T("controls.tree.nodestyle"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyleHor				, _T("controls.tree.nodestyle.horizattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyleDot				, _T("controls.tree.nodestyle.dotlineattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyleFolder			, _T("controls.tree.nodestyle.folderattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyleCheckBox		, _T("controls.tree.nodestyle.checkboxattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pTreeNodeStyleItem			, _T("controls.tree.nodestyle.itemattr"));
	DUI_GET_CONTROL_PTR(DuiLib::CListUI				, __m_pAttribList					, _T("control.attr.list"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pAttribListItemStyle			, _T("control.attr.list.itemstyle"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pAttribListItemStyleName		, _T("control.attr.list.itemstyle.name.richedit"));
	DUI_GET_CONTROL_PTR(DuiLib::CControlUI			, __m_pAttribListItemStyleType		, _T("control.attr.list.itemstyle.type.combo"));
	DUI_GET_CONTROL_PTR(DuiLib::CRichEditUI			, __m_pControlAliasRich				, _T("control.attr.alias.richedit"));
	DUI_GET_CONTROL_PTR(DuiLib::CRichEditUI			, __m_pControlImageRich				, _T("control.attr.image.richedit"));
	DUI_GET_CONTROL_PTR(DuiLib::CComboUI			, __m_pParentCombo					, _T("control.attr.parent.combo"));
	DUI_GET_CONTROL_PTR(DuiLib::CVerticalLayoutUI	, __m_pControlAttr					, _T("control.attr.v"));
	__LoadConfig();
}

void CSettingsWnd::Notify( DuiLib::TNotifyUI& msg )
{
	ON_DUI_NOTIFY(__OnTypeChange				, _T("type")												, DUI_MSGTYPE_SELECTCHANGED);
	ON_DUI_NOTIFY(__OnControlsTreeSelectChange	, _T("controls.tree")										, DUI_MSGTYPE_ITEMSELECT);
	ON_DUI_NOTIFY(__OnControlsTreeSelectChange	, _T("control.attr.parent.combo")							, DUI_MSGTYPE_ITEMSELECT);
	ON_DUI_NOTIFY(__OnChangePreview				, _T("control.attr.list.item.alias.richedit")				, DUI_MSGTYPE_RETURN);
	ON_DUI_NOTIFY(__OnChangePreview				, _T("control.attr.list.item.alias.richedit")				, DUI_MSGTYPE_KILLFOCUS);
	ON_DUI_NOTIFY(__OnChangePreview				, _T("control.attr.list.item.type.combo")					, DUI_MSGTYPE_ITEMSELECT);
	ON_DUI_NOTIFY(__OnAttribAdd					, _T("control.attr.list.add.btn")							, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnAttribDelete				, _T("control.attr.list.delete.btn")						, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnControlEnable				, _T("controls.tree.node.checkbox")							, DUI_MSGTYPE_SELECTCHANGED);
	ON_DUI_NOTIFY(__OnAttrbMoveUp				, _T("control.attr.list.up.btn")							, DUI_MSGTYPE_CLICK);
	ON_DUI_NOTIFY(__OnAttrbMoveDown				, _T("control.attr.list.down.btn")							, DUI_MSGTYPE_CLICK);
	__super::Notify(msg);
}

LRESULT CSettingsWnd::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	DuiLib::CControlUI* pItem = __m_pControlsTree->GetItemAt(__m_pControlsTree->GetCurSel());
	if (pItem)
	{
		DuiLib::CTreeNodeUI* pNote = static_cast<DuiLib::CTreeNodeUI*>(pItem->GetInterface(DUI_CTR_TREENODE));
		if (pNote) __SaveAttribs(__m_pAttribList->GetUserData(), pNote->GetCheckBox()->GetCheck());
	}
	
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void CSettingsWnd::__LoadConfig()
{
	__InitBasic();
	__InitControls();
	__InitAttribs();
}

void CSettingsWnd::__OnTypeChange( DuiLib::TNotifyUI& msg )
{
	__m_pRootTab->SelectItem(_ttoi(msg.pSender->GetUserData()));
}

void CSettingsWnd::__InitBasic()
{

}

void CSettingsWnd::__InitControls()
{
	static const DuiLib::CDuiString szDefaultControls[] = 
	{
		_T("ChildLayout"),
		_T("Control"),
		_T("Container"),
		_T("VerticalLayout"),
		_T("HorizontalLayout"),
		_T("TileLayout"),
		_T("TabLayout"),
		_T("ActiveX"),
		_T("WebBrowser"),
		_T("Combo"),
		_T("Label"),
		_T("Button"),
		_T("CheckBox"),
		_T("Option"),
		_T("Text"),
		_T("Progress"),
		_T("Slider"),
		_T("Edit"),
		_T("List"),
		_T("ListHeader"),
		_T("ListHeaderItem"),
		_T("ListLabelElement"),
		_T("ListTextElement"),
		_T("ListContainerElement"),
		_T("RichEdit"),
		_T("TreeView"),
		_T("TreeNode"),
		_T("DateTime"),
		STR_SCROLLBAR,
	};
	static const UINT unDefaultControlsSize = sizeof(szDefaultControls) / sizeof(DuiLib::CDuiString);
	DuiLib::CListLabelElementUI* pComboItem = new DuiLib::CListLabelElementUI;
	pComboItem->SetText(_T(""));
	__m_pParentCombo->Add(pComboItem);
	std::vector<DuiLib::CDuiString> vecControls;
	for (UINT i = 0; i < unDefaultControlsSize; ++i)
		vecControls.push_back(szDefaultControls[i]);
	std::sort(vecControls.begin(), vecControls.end());
	__m_pControlsTree->RemoveAll();
	DuiLib::CTreeNodeUI* pDefaultNode = __NewTreeNode(true);
	pDefaultNode->SetItemText(_T("д╛хо"));
	if (!pDefaultNode) return;
	for (std::vector<DuiLib::CDuiString>::iterator it = vecControls.begin(); it != vecControls.end(); ++it)
	{
		DuiLib::CTreeNodeUI* pNode = __NewTreeNode(false);
		if (!pNode) continue;
		pNode->GetFolderButton()->SetEnabled(false);
		pNode->SetItemText(*it);
		pNode->GetCheckBox()->SetUserData(*it);
		pNode->GetItemButton()->SetUserData(*it);
		pDefaultNode->Add(pNode);
		DuiLib::CListLabelElementUI* pComboItem = new DuiLib::CListLabelElementUI;
		pComboItem->SetText(*it);
		__m_pParentCombo->Add(pComboItem);
		_SETTINGS_CONTROL* pSC = config.AddControl(*it);
		if (pSC) pNode->GetCheckBox()->SetCheck(pSC->bEnable);
	}
	__m_pControlsTree->Add(pDefaultNode);
	for (UINT i = 0; i < plugin_control.GetSize(); ++i)
	{
		_CONTROL_PLUGIN* pPlugin = plugin_control.GetItem(i);
		if (pPlugin)
		{
			DuiLib::CTreeNodeUI* pPluginNode = __NewTreeNode(true);
			if (!pPluginNode) continue;;
			pPluginNode->SetItemText(pPlugin->strFile);
			vecControls = pPlugin->vecControls;
			std::sort(vecControls.begin(), vecControls.end());
			for (std::vector<DuiLib::CDuiString>::iterator it = vecControls.begin(); it != vecControls.end(); ++it)
			{
				DuiLib::CTreeNodeUI* pNode = __NewTreeNode(false);
				if (!pNode) continue;
				pNode->GetFolderButton()->SetEnabled(false);
				pNode->SetItemText(*it);
				pNode->GetCheckBox()->SetUserData(*it);
				pNode->GetItemButton()->SetUserData(*it);
				pPluginNode->Add(pNode);
				DuiLib::CListLabelElementUI* pComboItem = new DuiLib::CListLabelElementUI;
				pComboItem->SetText(*it);
				__m_pParentCombo->Add(pComboItem);
				_SETTINGS_CONTROL* pSC = config.AddControl(*it);
				if (pSC) pNode->GetCheckBox()->SetCheck(pSC->bEnable);
			}
			__m_pControlsTree->Add(pPluginNode);
		}
	}
	__m_pControlsTree->ApplyAttributeList(__m_pControlsTree->GetUserData());
}

void CSettingsWnd::__InitAttribs()
{
	for (UINT i = 0; i < plugin_attrib.GetSize(); ++i)
	{
		_ATTRIB_PLUGIN* pPlugin = plugin_attrib.GetItem(i);
		for (auto it = pPlugin->vecAttribs.begin(); it != pPlugin->vecAttribs.end(); ++it)
			__m_vecAttribType.push_back(*it);
	}
}

bool CSettingsWnd::__AddAttrib( _SETTINGS_ATTRIB& _sa, int _nIndex, bool _bEdit )
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("SettingsAttrListItem.xml"), NULL, this, &m_PaintManager);
	DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pName = pContainer->FindSubControl(_T("control.attr.list.item.name.richedit"));
	DuiLib::CControlUI* pAlias = pContainer->FindSubControl(_T("control.attr.list.item.alias.richedit"));
	DuiLib::CControlUI* pDefaultValue = pContainer->FindSubControl(_T("control.attr.list.item.defaultvalue.richedit"));
	pName->SetText(_sa.strName);
	pName->SetTag(_nIndex);
	pAlias->SetText(_sa.strAlias);
	pAlias->SetTag(_nIndex);
	pDefaultValue->SetText(_sa.strDefaultValue);
	pDefaultValue->SetTag(_nIndex);
	DuiLib::CComboUI* pType = static_cast<DuiLib::CComboUI*>(pContainer->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));
	pType->SetTag(_nIndex);
	pName->SetMouseEnabled(_bEdit);
	pAlias->SetMouseEnabled(_bEdit);
	pType->SetMouseEnabled(_bEdit);
	pControl->SetMouseEnabled(_bEdit);
	for (auto itType = __m_vecAttribType.begin(); itType != __m_vecAttribType.end(); ++itType)
	{
		DuiLib::CListLabelElementUI* pItem = new DuiLib::CListLabelElementUI;
		pItem->SetText(*itType);
		pType->Add(pItem);
		if (*itType == _sa.strType) 
			pType->SelectItem(pType->GetCount() - 1);
	}
	DuiLib::CContainerUI* pPreView = static_cast<DuiLib::CContainerUI*>(pContainer->FindSubControl(_T("control.attr.list.item.preview.h"))->GetInterface(DUI_CTR_CONTAINER));
	if (pPreView)
	{
		DuiLib::CControlUI* pAttrControl = plugin_attrib.CreateAttrib(_sa.strType, nullptr, _sa.strName, _sa.strAlias);
		if (pAttrControl)
		{
			pPreView->Add(pAttrControl);
			pControl->SetFixedHeight(pAttrControl->GetFixedHeight() + 10);
		}
	}
	if (!__m_pAttribList->AddAt(pControl, _nIndex))
	{
		delete pControl;
		return false;
	}
	return true;
}

DuiLib::CTreeNodeUI* CSettingsWnd::__NewTreeNode(bool _bFolder)
{
	DuiLib::CTreeNodeUI* pNode = new DuiLib::CTreeNodeUI;
	if (__m_pTreeNodeStyle) pNode->ApplyAttributeList(__m_pTreeNodeStyle->GetUserData());
	if (__m_pTreeNodeStyleHor) pNode->SetAttribute(_T("horizattr"), __m_pTreeNodeStyleHor->GetUserData());
	if (__m_pTreeNodeStyleDot) pNode->SetAttribute(_T("dotlineattr"), __m_pTreeNodeStyleDot->GetUserData());
	if (__m_pTreeNodeStyleFolder && _bFolder) pNode->SetAttribute(_T("folderattr"), __m_pTreeNodeStyleFolder->GetUserData());
	if (__m_pTreeNodeStyleCheckBox) pNode->SetAttribute(_T("checkboxattr"), __m_pTreeNodeStyleCheckBox->GetUserData());
	if (__m_pTreeNodeStyleItem) pNode->SetAttribute(_T("itemattr"), __m_pTreeNodeStyleItem->GetUserData());
	return pNode;
}

void CSettingsWnd::__LoadAttrib( LPCTSTR _lpName, bool _bEdit )
{
	_SETTINGS_CONTROL* pSC = config.AddControl(_lpName);
	if (!pSC) return;
	for (_SETTINGS_ATTRIBS::iterator itAttr = pSC->vecAttrib.begin(); itAttr != pSC->vecAttrib.end(); ++itAttr)
		__AddAttrib(*itAttr, __m_pAttribList->GetCount(), _bEdit);
	if (_bEdit)
	{
		__m_pControlAliasRich->SetText(pSC->strClassName);
		__m_pControlImageRich->SetText(pSC->strImage);
		__SelectCombo(__m_pParentCombo, pSC->strParent, pSC->strName);
	}
	//if (!pSC->strParent.IsEmpty()) __LoadAttrib(pSC->strParent, false);
}

void CSettingsWnd::__OnControlsTreeSelectChange( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pItem = __m_pControlsTree->GetItemAt(__m_pControlsTree->GetCurSel());
	if (!pItem)
		return;
	DuiLib::CTreeNodeUI* pNote = static_cast<DuiLib::CTreeNodeUI*>(pItem->GetInterface(DUI_CTR_TREENODE));
	if (pNote->GetFolderButton()->IsEnabled()) 
	{
		__m_pControlAttr->SetVisible(false);
		return;
	}
	__SaveAttribs(__m_pAttribList->GetUserData(), pNote->GetCheckBox()->GetCheck());
	__m_pAttribList->RemoveAll();
	__m_pAttribList->SetUserData(pNote->GetItemButton()->GetText());
	__m_pControlAttr->SetVisible(true);
	__LoadAttrib(pNote->GetItemButton()->GetText(), true);
}

void CSettingsWnd::__OnChangePreview( DuiLib::TNotifyUI& msg )
{
	DuiLib::CControlUI* pControl = __m_pAttribList->GetItemAt(msg.pSender->GetTag());
	if (!pControl) return;
	DuiLib::CContainerUI* pItem = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	if (!pItem) return;
	DuiLib::CControlUI* pName = pItem->FindSubControl(_T("control.attr.list.item.name.richedit"));
	DuiLib::CControlUI* pAlias = pItem->FindSubControl(_T("control.attr.list.item.alias.richedit"));
	DuiLib::CComboUI* pType = static_cast<DuiLib::CComboUI*>(pItem->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));
	DuiLib::CContainerUI* pPreView = static_cast<DuiLib::CContainerUI*>(pItem->FindSubControl(_T("control.attr.list.item.preview.h"))->GetInterface(DUI_CTR_CONTAINER));
	pPreView->RemoveAll();
	DuiLib::CControlUI* pAttrControl = plugin_attrib.CreateAttrib(pType->GetText(), nullptr, pName->GetText(), pAlias->GetText());
	if (pAttrControl)
	{
		pPreView->Add(pAttrControl);
		pItem->SetFixedHeight(pAttrControl->GetFixedHeight() + 10);
	}
}

void CSettingsWnd::__SaveAttribs( DuiLib::CDuiString _strControl, bool _bEnable )
{
	if (_strControl.IsEmpty() || !__m_pControlAttr->IsVisible()) return;
	_SETTINGS_CONTROL* pSC = config.AddControl(_strControl);
	pSC->strClassName = __m_pControlAliasRich->GetText();
	pSC->strParent = __m_pParentCombo->GetText();
	pSC->strImage = __m_pControlImageRich->GetText();
	pSC->bEnable = _bEnable;
	if (pSC->strParent == pSC->strName) pSC->strParent.Empty();
	pSC->vecAttrib.clear();
	for (int i = 0; i < __m_pAttribList->GetCount(); ++i)
	{
		DuiLib::CControlUI* pControl = __m_pAttribList->GetItemAt(i);
		if (!pControl || !pControl->IsMouseEnabled()) continue;
		DuiLib::CContainerUI* pItem = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
		if (!pItem) continue;
		DuiLib::CControlUI* pName = pItem->FindSubControl(_T("control.attr.list.item.name.richedit"));
		DuiLib::CControlUI* pAlias = pItem->FindSubControl(_T("control.attr.list.item.alias.richedit"));
		DuiLib::CControlUI* pDefaultValue = pItem->FindSubControl(_T("control.attr.list.item.defaultvalue.richedit"));
		DuiLib::CComboUI* pType = static_cast<DuiLib::CComboUI*>(pItem->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));

		_SETTINGS_ATTRIB sa;
		sa.strName = pName->GetText();
		sa.strType = pType->GetText();
		sa.strAlias = pAlias->GetText();
		sa.strDefaultValue = pDefaultValue->GetText();
		pSC->vecAttrib.push_back(sa);
	}
}

void CSettingsWnd::__OnAttribAdd( DuiLib::TNotifyUI& msg )
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("SettingsAttrListItem.xml"), NULL, this, &m_PaintManager);
	DuiLib::CContainerUI* pContainer = static_cast<DuiLib::CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
	DuiLib::CControlUI* pName = pContainer->FindSubControl(_T("control.attr.list.item.name.richedit"));
	DuiLib::CComboUI* pType = static_cast<DuiLib::CComboUI*>(pContainer->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));
	for (auto itType = __m_vecAttribType.begin(); itType != __m_vecAttribType.end(); ++itType)
	{
		DuiLib::CListLabelElementUI* pItem = new DuiLib::CListLabelElementUI;
		pItem->SetText(*itType);
		pType->Add(pItem);
	}

	int nPos = __m_pAttribList->GetCurSel() + 1;
	if (!__m_pAttribList->AddAt(pControl, nPos))
		__m_pAttribList->Add(pControl);
	pName->SetTag(__m_pAttribList->GetItemIndex(pControl));
	pType->SetTag(__m_pAttribList->GetItemIndex(pControl));
}

void CSettingsWnd::__OnAttribDelete( DuiLib::TNotifyUI& msg )
{
	int nPos = __m_pAttribList->GetCurSel();
	__m_pAttribList->RemoveAt(nPos);
}

void CSettingsWnd::__OnControlEnable( DuiLib::TNotifyUI& msg )
{
	DuiLib::CDuiString strControl = msg.pSender->GetUserData();
	if (strControl.IsEmpty()) 
		return;
	_SETTINGS_CONTROL* pSC = config.AddControl(strControl);
	DuiLib::CCheckBoxUI* pCheck = static_cast<DuiLib::CCheckBoxUI*>(msg.pSender->GetInterface(DUI_CTR_CHECKBOX));
	if (pCheck && pSC)
	{
		pSC->bEnable = pCheck->GetCheck();
	}
}

void CSettingsWnd::__OnAttrbMoveUp( DuiLib::TNotifyUI& msg )
{
	int nPos = __m_pAttribList->GetCurSel();
	if (nPos < 1) return;
	__SwapAttrib(nPos, nPos - 1);
	__m_pAttribList->SelectItem(nPos - 1);
}

void CSettingsWnd::__OnAttrbMoveDown( DuiLib::TNotifyUI& msg )
{
	int nPos = __m_pAttribList->GetCurSel();
	if (nPos < 0 || nPos >= __m_pAttribList->GetCount() - 1) return;
	__SwapAttrib(nPos, nPos + 1);
	__m_pAttribList->SelectItem(nPos + 1);
}

void CSettingsWnd::__SwapAttrib( int _nPos1, int _nPos2 )
{
	DuiLib::CControlUI* pControl1 = __m_pAttribList->GetItemAt(_nPos1);
	if (!pControl1) return;
	DuiLib::CContainerUI* pItem1 = static_cast<DuiLib::CContainerUI*>(pControl1->GetInterface(DUI_CTR_CONTAINER));
	if (!pItem1) return;
	DuiLib::CControlUI* pName1 = pItem1->FindSubControl(_T("control.attr.list.item.name.richedit"));
	DuiLib::CControlUI* pAlias1 = pItem1->FindSubControl(_T("control.attr.list.item.alias.richedit"));
	DuiLib::CControlUI* pValue1 = pItem1->FindSubControl(_T("control.attr.list.item.defaultvalue.richedit"));
	DuiLib::CComboUI* pType1 = static_cast<DuiLib::CComboUI*>(pItem1->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));
	DuiLib::CContainerUI* pPreView1 = static_cast<DuiLib::CContainerUI*>(pItem1->FindSubControl(_T("control.attr.list.item.preview.h"))->GetInterface(DUI_CTR_CONTAINER));

	DuiLib::CControlUI* pControl2 = __m_pAttribList->GetItemAt(_nPos2);
	if (!pControl2) return;
	DuiLib::CContainerUI* pItem2 = static_cast<DuiLib::CContainerUI*>(pControl2->GetInterface(DUI_CTR_CONTAINER));
	if (!pItem2) return;
	DuiLib::CControlUI* pName2 = pItem2->FindSubControl(_T("control.attr.list.item.name.richedit"));
	DuiLib::CControlUI* pAlias2 = pItem2->FindSubControl(_T("control.attr.list.item.alias.richedit"));
	DuiLib::CControlUI* pValue2 = pItem2->FindSubControl(_T("control.attr.list.item.defaultvalue.richedit"));
	DuiLib::CComboUI* pType2 = static_cast<DuiLib::CComboUI*>(pItem2->FindSubControl(_T("control.attr.list.item.type.combo"))->GetInterface(DUI_CTR_COMBO));
	DuiLib::CContainerUI* pPreView2 = static_cast<DuiLib::CContainerUI*>(pItem2->FindSubControl(_T("control.attr.list.item.preview.h"))->GetInterface(DUI_CTR_CONTAINER));

	DuiLib::CDuiString strName = pName1->GetText();
	DuiLib::CDuiString strAlias = pAlias1->GetText();
	DuiLib::CDuiString strValue = pValue1->GetText();
	DuiLib::CDuiString strType = pType1->GetText();

	pName1->SetText(pName2->GetText()); 
	pAlias1->SetText(pAlias2->GetText()); 
	pValue1->SetText(pValue2->GetText()); 
	__SelectCombo(pType1, pType2->GetText()); 

	pName2->SetText(strName);
	pAlias2->SetText(strAlias);
	pValue2->SetText(strValue);
	__SelectCombo(pType2, strType);

	pPreView1->RemoveAll();
	DuiLib::CControlUI* pAttrControl1 = plugin_attrib.CreateAttrib(pType1->GetText(), nullptr, pName1->GetText(), pAlias1->GetText());
	if (pAttrControl1)
	{
		pPreView1->Add(pAttrControl1);
		pItem1->SetFixedHeight(pAttrControl1->GetFixedHeight() + 10);
	}

	pPreView2->RemoveAll();
	DuiLib::CControlUI* pAttrControl2 = plugin_attrib.CreateAttrib(pType2->GetText(), nullptr, pName2->GetText(), pAlias2->GetText());
	if (pAttrControl2)
	{
		pPreView2->Add(pAttrControl2);
		pItem2->SetFixedHeight(pAttrControl2->GetFixedHeight() + 10);
	}

}

void CSettingsWnd::__SelectCombo( DuiLib::CComboUI* pCombo, LPCTSTR _lpStr, LPCTSTR _lpHide )
{
	if (!pCombo || !_lpStr) return;
	for (int i = 0; i < pCombo->GetCount(); ++i)
	{
		DuiLib::CControlUI* pItem = pCombo->GetItemAt(i);
		if (_lpHide) pItem->SetVisible(pItem->GetText() != _lpHide);
		if (pItem->GetText() == _lpStr) pCombo->SelectItem(i);
	}
}
